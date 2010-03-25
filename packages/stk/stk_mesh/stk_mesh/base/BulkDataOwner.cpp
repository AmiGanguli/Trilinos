/*------------------------------------------------------------------------*/
/*                 Copyright 2010 Sandia Corporation.                     */
/*  Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive   */
/*  license for use of this work by or on behalf of the U.S. Government.  */
/*  Export of this program may require a license from the                 */
/*  United States Government.                                             */
/*------------------------------------------------------------------------*/

/**
 * @author H. Carter Edwards
 */

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>

#include <stk_util/parallel/ParallelComm.hpp>
#include <stk_util/parallel/ParallelReduce.hpp>

#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/MetaData.hpp>
#include <stk_mesh/base/Entity.hpp>
#include <stk_mesh/base/EntityComm.hpp>

namespace stk {
namespace mesh {

//----------------------------------------------------------------------

namespace {

void insert_closure_ghost( const MetaData & meta ,
                           Entity * const entity ,
                           std::set<Entity*,EntityLess> & remove_list )
{
  if ( ! entity->bucket().member( meta.locally_used_part() ) ) {
    // This entity is a ghost, put it on the remove_list
    // along with all ghosts in its closure

    std::pair< std::set<Entity*,EntityLess>::iterator , bool >
      result = remove_list.insert( entity );

    if ( result.second ) {
      // This ghost entity is new to the list, traverse its closure.

      const unsigned etype = entity->entity_type();

      for ( PairIterRelation
            irel = entity->relations() ; ! irel.empty() ; ++irel ) {

        if ( irel->entity_rank() < etype ) {
          insert_closure_ghost( meta , irel->entity() , remove_list );
        }
      }
    }
  }
}

void insert_transitive_ghost( const MetaData & meta ,
                              Entity * const entity ,
                              std::set<Entity*,EntityLess> & remove_list )
{
  insert_closure_ghost( meta , entity , remove_list );

  // Transitive:
  // If this entity is a member of another entity's closure
  // then that other entity is part of the traversal.

  const unsigned etype = entity->entity_type();

  for ( PairIterRelation rel = entity->relations(); ! rel.empty() ; ++rel ) {
    if ( etype < rel->entity_rank() ) {
      insert_transitive_ghost( meta , rel->entity() , remove_list );
    }
  }
}

//----------------------------------------------------------------------

void insert_closure_send(
  const EntityProc                  send_entry ,
  std::set<EntityProc,EntityLess> & send_list )
{
  std::pair< std::set<EntityProc,EntityLess>::iterator , bool >
    result = send_list.insert( send_entry );

  if ( result.second ) {
    // First time this entity was inserted into the send_list.

    const unsigned etype  = send_entry.first->entity_type();
    PairIterRelation irel = send_entry.first->relations();

    for ( ; ! irel.empty() ; ++irel ) {
      if ( irel->entity_rank() < etype ) {
        const EntityProc rel_send_entry( irel->entity(), send_entry.second );

        insert_closure_send( rel_send_entry , send_list );
      }
    }
  }
}

//----------------------------------------------------------------------

bool member_of_owned_closure( const Entity & e , const unsigned p_rank )
{
  bool result = p_rank == e.owner_rank();

  const unsigned etype = e.entity_type();

  // Any higher ranking entities locally owned?
  for ( PairIterRelation
        irel = e.relations(); ! result && ! irel.empty() ; ++irel ) {
    result = etype  <  irel->entity_rank() &&
             p_rank == irel->entity()->owner_rank();
  }

  // Any higher ranking entity member of an owned closure?
  for ( PairIterRelation
        irel = e.relations(); ! result && ! irel.empty() ; ++irel ) {
    result = etype < irel->entity_rank() &&
             member_of_owned_closure( * irel->entity() , p_rank );
  }

  return result ;
}

//----------------------------------------------------------------------

void clean_and_verify_parallel_change(
  const char method[] ,
  const BulkData & mesh ,
  std::vector<EntityProc> & local_change )
{
  const MetaData      & meta   = mesh.mesh_meta_data() ;
  const unsigned        p_rank = mesh.parallel_rank();
  const unsigned        p_size = mesh.parallel_size();
  const ParallelMachine p_comm = mesh.parallel();

  size_t error_count = 0 ;

  std::ostringstream error_msg ;

  // Order and eliminate redundancies:
  {
    std::vector<EntityProc>::iterator i = local_change.begin() ,
                                      j = local_change.end() ;
    std::sort( i , j , EntityLess() );
    i = std::unique( i , j );
    local_change.erase( i , j );
  }

  for ( std::vector<EntityProc>::iterator 
        i = local_change.begin() ; i != local_change.end() ; ) {
    std::vector<EntityProc>::iterator j = i ; ++i ;
    Entity * const entity    = j->first ;
    const unsigned new_owner = j->second ;
  
    // Verification:
    // 1) If bucket has no capacity then is destined for deletion
    // 2) If not locally owned then not allowed grant ownership
    // 3) New owner must be legit
    // 4) Cannot grant to two different owners

    const bool bad_delete = 0 == entity->bucket().capacity();
    const bool bad_entity = entity->owner_rank() != p_rank ;
    const bool bad_owner  = p_size <= new_owner ;
    const bool bad_dup    = i != local_change.end() && entity == i->first ;

    if ( bad_entity || bad_owner || bad_dup || bad_delete ) {
      ++error_count ;

      error_msg << "  P" << p_rank << ": " ;
      print_entity_key( error_msg , meta , entity->key() );
      if ( bad_delete ) { error_msg << " HAS_BEEN_DELETED" ; }
      if ( bad_entity ) { error_msg << " NOT_CURRENT_OWNER" ; }
      if ( bad_owner ) {
        error_msg << " BAD_NEW_OWNER( " << new_owner << " )" ;
       }
      if ( bad_dup ) {
        error_msg << " CONFLICTING_NEW_OWNER( " << new_owner ;
        error_msg << " != " << i->second << " )" ;
      }
      error_msg << std::endl ;
    }
    else if ( new_owner == p_rank ) {
      // Eliminate non-changes
      j->first = NULL ;
      j->second = 0 ;
    }
  }

  all_reduce( p_comm , ReduceSum<1>( & error_count ) );

  if ( error_count ) {
    std::string msg_throw ;
    msg_throw.append( method );
    msg_throw.append( " FAILED: Bad change ownership directives" );

    if ( 0 == p_rank ) { std::cerr << msg_throw ; }

    all_write_string( p_comm , std::cerr , error_msg.str() );

    throw std::runtime_error( msg_throw );
  }

  {
    std::vector<EntityProc>::iterator i = local_change.begin(),
                                      j = local_change.end();
    i = std::remove( i , j , EntityProc(NULL,0) );
    local_change.erase( i , j );
  }
}

//----------------------------------------------------------------------
// Generate a parallel consistent list of ownership changes:
// 1) Shared entities (not owned but in closure of an owned entity),
// 2) Ghosted entities (not owned and not in closure of an owned entity), and
// 3) Parallel index.

void generate_parallel_change( const BulkData & mesh ,
                               const std::vector<EntityProc> & local_change ,
                                     std::vector<EntityProc> & shared_change ,
                                     std::vector<EntityProc> & ghosted_change )
{
  const unsigned p_size = mesh.parallel_size();

  CommAll comm( mesh.parallel() );

  // Sizing:

  std::vector<unsigned> procs ;

  for ( std::vector<EntityProc>::const_iterator
        ip = local_change.begin() ; ip != local_change.end() ; ++ip ) {
    Entity & entity = * ip->first ;
    comm_procs( entity , procs );
    for ( std::vector<unsigned>::iterator 
           j = procs.begin() ; j != procs.end() ; ++j ) {
      comm.send_buffer( *j ).skip<EntityKey>(1).skip<unsigned>(1);
    }
  }

  // Allocation:

  comm.allocate_buffers( p_size / 4 , 0 );

  // Packing new owner info:

  for ( std::vector<EntityProc>::const_iterator
        ip = local_change.begin() ; ip != local_change.end() ; ++ip ) {
    Entity & entity = * ip->first ;
    comm_procs( entity , procs );
    for ( std::vector<unsigned>::iterator 
           j = procs.begin() ; j != procs.end() ; ++j ) {
      comm.send_buffer( *j )
          .pack<EntityKey>( entity.key() )
          .pack<unsigned>(  ip->second );
    }
  }

  comm.communicate();

  for ( unsigned ip = 0 ; ip < p_size ; ++ip ) {
    CommBuffer & buf = comm.recv_buffer( ip );
    while ( buf.remaining() ) {
      EntityProc entry ;
      EntityKey key ;
      buf.unpack<EntityKey>( key )
         .unpack<unsigned>( entry.second );

      entry.first = mesh.get_entity( key );

      if ( in_receive_ghost( * entry.first ) ) {
        ghosted_change.push_back( entry );
      }
      else {
        shared_change.push_back( entry );
      }
    }
  }

  std::sort( shared_change.begin() , shared_change.end() , EntityLess() );
  std::sort( ghosted_change.begin() , ghosted_change.end() , EntityLess() );
}

}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

void BulkData::change_entity_owner( const std::vector<EntityProc> & arg_change )
{
  static const char method[] = "stk::mesh::BulkData::change_entity_owner" ;

  const MetaData  & meta = m_mesh_meta_data ;
  const unsigned  p_rank = m_parallel_rank ;
  const unsigned  p_size = m_parallel_size ;
  ParallelMachine p_comm = m_parallel_machine ;

  //------------------------------
  // Verify the input changes, generate a clean local change list, and
  // generate the remote change list so that all processes know about
  // pending changes.

  std::vector<EntityProc> local_change( arg_change );

  // Parallel synchronous clean up and verify the requested changes:
  clean_and_verify_parallel_change( method , *this , local_change );

  std::vector<EntityProc> ghosted_change ;
  std::vector<EntityProc> shared_change ;

  //----------------------------------------
  // Parallel synchronous determination of changing
  // shared and ghosted.

  generate_parallel_change( *this , local_change ,
                            shared_change , ghosted_change );

  //------------------------------
  // Have enough information to delete all effected ghosts.
  // If the closure of a ghost contains a changing entity
  // then that ghost must be deleted.
  // Request that all ghost entities in the closure of the ghost be deleted.

  {
    std::set<Entity*,EntityLess> work ;

    for ( std::vector<EntityProc>::const_iterator
          i = ghosted_change.begin() ; i != ghosted_change.end() ; ++i ) {
      insert_transitive_ghost( meta , i->first , work );
    }

    for ( std::vector<EntityProc>::const_iterator
          i = shared_change.begin() ; i != shared_change.end() ; ++i ) {
      insert_transitive_ghost( meta , i->first , work );
    }

    for ( std::vector<EntityProc>::const_iterator
          i = local_change.begin() ; i != local_change.end() ; ++i ) {
      insert_transitive_ghost( meta , i->first , work );
    }

    // The ghosted change list will become invalid
    ghosted_change.clear();

    std::vector<EntityProc> empty ;
    std::vector<Entity*> effected_ghosts( work.begin() , work.end() );

    // Skip 'm_ghosting[0]' which is the shared subset.
    for ( std::vector<Ghosting*>::iterator
          ig = m_ghosting.begin() + 1 ; ig != m_ghosting.end() ; ++ig ) {
      // parallel synchronous:
      internal_change_ghosting( **ig , empty , effected_ghosts );
    }
  }

  //------------------------------
  // Consistently change the owner on all processes.
  // 1) The local_change list is giving away ownership.
  // 2) The shared_change may or may not be receiving ownership

  {
    PartVector owned( 1 );
    owned[0] = & meta.locally_owned_part();

    for ( std::vector<EntityProc>::iterator
          i = local_change.begin() ; i != local_change.end() ; ++i ) {
      // Giving ownership, change the parts first and then
      // the owner rank to pass the ownership test.
      change_entity_parts( * i->first , PartVector() , owned );
      i->first->m_owner_rank = i->second ;
    }

    for ( std::vector<EntityProc>::iterator
          i = shared_change.begin() ; i != shared_change.end() ; ++i ) {
      i->first->m_owner_rank = i->second ;
      if ( p_rank == i->second ) { // I receive ownership
        change_entity_parts( * i->first , owned , PartVector() );
      }
    }
  }

  //------------------------------
  // Send entities, along with their closure, to the new owner processes
  // Remember what is destroyed and created to update the distributed index.
  {
    std::vector< parallel::DistributedIndex::KeyType > 
      distributed_index_add , distributed_index_remove ;

    std::set< EntityProc , EntityLess > send_closure ;
    std::ostringstream error_msg ;
    int error_count = 0 ;

    for ( std::vector<EntityProc>::iterator
          i = local_change.begin() ; i != local_change.end() ; ++i ) {
      insert_closure_send( *i , send_closure );
    }

    CommAll comm( p_comm );

    for ( std::set<EntityProc,EntityLess>::iterator
          i = send_closure.begin() ; i != send_closure.end() ; ++i ) {
      CommBuffer & buffer = comm.send_buffer( i->second );
      Entity     & entity = * i->first ;
      pack_entity_info( buffer , entity );
      pack_field_values( buffer , entity );
    }

    comm.allocate_buffers( p_size / 4 );

    for ( std::set<EntityProc,EntityLess>::iterator
          i = send_closure.begin() ; i != send_closure.end() ; ++i ) {
      CommBuffer & buffer = comm.send_buffer( i->second );
      Entity     & entity = * i->first ;
      pack_entity_info( buffer , entity );
      pack_field_values( buffer , entity );
    }

    // Any entity that I sent and is not in an owned closure is deleted.
    // Destroy backwards so as not to invalidate closures in the process.

    for ( std::set<EntityProc,EntityLess>::iterator
          i = send_closure.end() ; i != send_closure.begin() ; ) {
      --i ;
      if ( ! member_of_owned_closure( * i->first , p_rank ) ) {
        // Need to remove this entry from the distributed index
        parallel::DistributedIndex::KeyType tmp = i->first->key().raw_key();
        distributed_index_remove.push_back( tmp );

        // Now destroy the entity.
        Entity * e = i->first ;
        if ( ! destroy_entity( e ) ) {
          throw std::logic_error(std::string("BulkData::destroy_entity FAILED"));
        }
      }
    }

    send_closure.clear(); // Has been invalidated

    comm.communicate();

    for ( unsigned p = 0 ; p < p_size ; ++p ) {
      CommBuffer & buf = comm.recv_buffer(p);
      while ( buf.remaining() ) {
        PartVector parts ;
        std::vector<Relation> relations ;
        EntityKey key ;
        unsigned  owner = ~0u ;

        unpack_entity_info( buf, *this, key, owner, parts, relations );

        // Received entity information will be correct, modulo the owned part
        if ( owner == p_rank ) {
          // Must have the locally_owned_part
          insert( parts , meta.locally_owned_part() );
        }
        else {
          // Must not have the locally_owned_part
          remove( parts , meta.locally_owned_part() );
        }

        std::pair<Entity*,bool> result = internal_create_entity( key );

        result.first->m_owner_rank = owner ;

        internal_change_entity_parts( *result.first , parts , PartVector() );

        declare_relation( *result.first , relations );

        if ( ! unpack_field_values( buf , * result.first , error_msg ) ) {
          ++error_count ;
        }

        // Successfully added entity with 'key',
        // need to add this to the distributed index
        parallel::DistributedIndex::KeyType tmp = key.raw_key();
        distributed_index_add.push_back( tmp );
      }
    }

    all_reduce( p_comm , ReduceSum<1>( & error_count ) );

    if ( error_count ) { throw std::runtime_error( error_msg.str() ); }

    m_entities_index.update_keys( distributed_index_add ,
                                  distributed_index_remove );
  }

  //------------------------------
  // Regenerate sharing information for existing entities.

  {
    std::vector< parallel::DistributedIndex::KeyProc >  sharing ;

    m_entities_index.query( sharing );

    bool added = false ;

    // 'sharing' is sorted by key and then process
    for ( std::vector< parallel::DistributedIndex::KeyProc >::iterator
          j = sharing.begin() ; j != sharing.end() ; ) {

      const std::vector< parallel::DistributedIndex::KeyProc >::iterator i = j ;
      const EntityKey key( & i->first );
      Entity & entity = * get_entity( key );

      std::vector< EntityCommInfo > entity_comm ;

      // Revised sharing:
      for ( ; j != sharing.end() && i->first == j->first ; ++j ) {
        if ( (int) p_rank != j->second ) {
          entity_comm.push_back( EntityCommInfo( 0 , j->second ) );
        }
      }

      // Current ghosting:
      {
        std::vector< EntityCommInfo >::iterator k = entity.m_comm.begin();
        for ( ; k != entity.m_comm.end() && k->ghost_id == 0 ; ++k );
        entity_comm.insert( entity_comm.end() , k , entity.m_comm.end() );
      }

      if ( entity_comm != entity.m_comm ) {
        if ( entity.m_comm.empty() && ! entity_comm.empty() ) {
          // Add to communicated entity list
          m_entity_comm.push_back( & entity );
          added = true ;
        }

        entity.m_comm.swap( entity_comm );
      }
    }

    {
      bool removed = false ;
      std::vector<Entity*>::iterator i = m_entity_comm.begin();
      for ( ; i != m_entity_comm.end() ; ++i ) {
        if ( (**i).m_comm.empty() ) { *i = NULL ; removed = true ; }
      }
      if ( removed ) {
        i = std::remove( m_entity_comm.begin() ,
                         m_entity_comm.end() , (Entity*) NULL );
        m_entity_comm.erase( i , m_entity_comm.end() );
      }
    }

    if ( added ) {
      std::sort( m_entity_comm.begin(), m_entity_comm.end(), EntityLess() );
    }
  }
}

//----------------------------------------------------------------------

} // namespace mesh
} // namespace stk

