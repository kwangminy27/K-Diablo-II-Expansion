#include "KEngine.h"
#include "replication_manager.h"

#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/Actor/actor.h"
#include "registry_manager.h"
#include "connection_manager.h"

void K::ReplicationManager::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ReplicationManager::Initialize" << std::endl;
	}
}

void K::ReplicationManager::SendActor(int _id)
{
	for (auto& actor : replicable_actor_list_)
	{
		OutputMemoryStream omstream{};

		auto tag = actor->tag();
		omstream.Serialize(tag.first);
		omstream.Serialize(tag.second);

		actor->Serialize(omstream);

		K::SC_PACKET_REPLICATION sc_packet_replication{};
		sc_packet_replication.size = static_cast<uint8_t>(sizeof(K::SC_PACKET_REPLICATION) + omstream.head());
		sc_packet_replication.type = K::SC_REPLICATION_CREATE;

		auto const& connection = ConnectionManager::singleton()->connection_vector().at(_id);
		connection->tcp_socket->Send(&sc_packet_replication, sizeof(K::SC_PACKET_REPLICATION));
		connection->tcp_socket->Send(omstream.buffer()->data(), omstream.head());
	}
}

void K::ReplicationManager::ReceiveActor(InputMemoryStream& _imstream)
{
	auto const& world_manager = WorldManager::singleton();
	auto const& registry_manager = RegistryManager::singleton();

	auto const& layer = world_manager->FindLayer({ "DefaultLayer", 1 });

	TAG tag{};
	_imstream.Serialize(tag.first);
	_imstream.Serialize(tag.second);

	auto actor = registry_manager->FindActorGenerator(tag.first)(tag);
	actor->Serialize(_imstream);

	layer->AddActor(actor);
}

void K::ReplicationManager::AddActor(APTR const& _actor)
{
	replicable_actor_list_.push_back(_actor);
}

void K::ReplicationManager::RemoveActor(APTR const& _actor)
{
	replicable_actor_list_.remove(_actor);
}

void K::ReplicationManager::_Finalize()
{
}
