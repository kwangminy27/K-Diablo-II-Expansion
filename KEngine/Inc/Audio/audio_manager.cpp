#include "KEngine.h"
#include "audio_manager.h"

#include "path_manager.h"

std::unique_ptr<DirectX::SoundEffect> K::AudioManager::sound_effect_dummy_{};
std::unique_ptr<DirectX::SoundEffectInstance> K::AudioManager::sound_effect_instance_dummy_{};

void K::AudioManager::Initialize()
{
	try
	{
		DirectX::AUDIO_ENGINE_FLAGS audio_engine_flags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
		audio_engine_flags = audio_engine_flags | DirectX::AudioEngine_Debug;
#endif
		audio_engine_ = std::make_unique<DirectX::AudioEngine>(audio_engine_flags);

		_CreateSoundEffect("BGM", L"town1.wav", AUDIO_PATH);
		_CreateSoundEffect("rain", L"rain2.wav", AUDIO_PATH);

		_CreateSoundEffect("sorceress_walk", L"LightDirt1.wav", AUDIO_PATH);
		_CreateSoundEffect("sorceress_run", L"LightDirtRun1.wav", AUDIO_PATH);

		_CreateSoundEffect("sorceress_gethit1", L"sorceress/soft1.wav", AUDIO_PATH);
		_CreateSoundEffect("sorceress_gethit2", L"sorceress/soft2.wav", AUDIO_PATH);
		_CreateSoundEffect("sorceress_gethit3", L"sorceress/soft3.wav", AUDIO_PATH);
		_CreateSoundEffect("sorceress_gethit4", L"sorceress/soft4.wav", AUDIO_PATH);
		_CreateSoundEffect("sorceress_gethit5", L"sorceress/soft5.wav", AUDIO_PATH);

		_CreateSoundEffect("ice_cast", L"sorceress/ice_cast.wav", AUDIO_PATH);
		_CreateSoundEffect("ice_bolt1", L"sorceress/ice_bolt1.wav", AUDIO_PATH);
		_CreateSoundEffect("ice_bolt2", L"sorceress/ice_bolt2.wav", AUDIO_PATH);
		_CreateSoundEffect("ice_bolt3", L"sorceress/ice_bolt3.wav", AUDIO_PATH);
		_CreateSoundEffect("ice_blast1", L"sorceress/ice_blast1.wav", AUDIO_PATH);
		_CreateSoundEffect("ice_blast2", L"sorceress/ice_blast2.wav", AUDIO_PATH);
		_CreateSoundEffect("ice_blast3", L"sorceress/ice_blast3.wav", AUDIO_PATH);
		_CreateSoundEffect("ice_orb", L"sorceress/ice_orb.wav", AUDIO_PATH);
		_CreateSoundEffect("frost_nova", L"sorceress/frost_nova.wav", AUDIO_PATH);
		_CreateSoundEffect("frozen_armor", L"sorceress/frozen_armor.wav", AUDIO_PATH);
		_CreateSoundEffect("teleport", L"sorceress/teleport.wav", AUDIO_PATH);

		_CreateSoundEffect("andariel_castsmall", L"andariel/castsmall.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_death", L"andariel/death.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_gethit1", L"andariel/gethit1.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_gethit2", L"andariel/gethit2.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_gethit3", L"andariel/gethit3.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_gethit4", L"andariel/gethit4.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit1", L"andariel/stonehit1.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit2", L"andariel/stonehit2.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit3", L"andariel/stonehit3.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit4", L"andariel/stonehit4.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit5", L"andariel/stonehit5.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit6", L"andariel/stonehit6.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit7", L"andariel/stonehit7.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit8", L"andariel/stonehit8.wav", AUDIO_PATH);
		_CreateSoundEffect("andariel_stonehit9", L"andariel/stonehit9.wav", AUDIO_PATH);

		_CreateSoundEffect("cow_attack1", L"cow/attack1.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_attack2", L"cow/attack2.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_attack3", L"cow/attack3.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_attack4", L"cow/attack4.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_attack5", L"cow/attack5.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_attack6", L"cow/attack6.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_death1", L"cow/death1.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_death2", L"cow/death2.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_death3", L"cow/death3.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_death4", L"cow/death4.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_death5", L"cow/death5.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_gethit1", L"cow/gethit1.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_gethit2", L"cow/gethit2.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_gethit3", L"cow/gethit3.wav", AUDIO_PATH);
		_CreateSoundEffect("cow_gethit4", L"cow/gethit4.wav", AUDIO_PATH);

		_CreateSoundEffect("wendigo_attack1", L"wendigo/attack1.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_attack2", L"wendigo/attack2.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_attack3", L"wendigo/attack3.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_death1", L"wendigo/death1.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_death2", L"wendigo/death2.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_death3", L"wendigo/death3.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_death4", L"wendigo/death4.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_gethit1", L"wendigo/gethit1.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_gethit2", L"wendigo/gethit2.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_gethit3", L"wendigo/gethit3.wav", AUDIO_PATH);
		_CreateSoundEffect("wendigo_gethit4", L"wendigo/gethit4.wav", AUDIO_PATH);

		_CreateSoundEffect("fallen_shaman_fireball", L"fallen shaman/fireball.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_death1", L"fallen shaman/death1.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_death2", L"fallen shaman/death2.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_death3", L"fallen shaman/death3.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_death4", L"fallen shaman/death4.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_gethit1", L"fallen shaman/gethit1.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_gethit2", L"fallen shaman/gethit2.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_gethit3", L"fallen shaman/gethit3.wav", AUDIO_PATH);
		_CreateSoundEffect("fallen_shaman_gethit4", L"fallen shaman/gethit4.wav", AUDIO_PATH);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "AudioManager::Initialize" << std::endl;
	}
}

void K::AudioManager::Update()
{
	audio_engine_->Update();
}

void K::AudioManager::Suspend()
{
	audio_engine_->Suspend();
}

void K::AudioManager::Resume()
{
	audio_engine_->Resume();
}

std::unique_ptr<DirectX::SoundEffect> const& K::AudioManager::FindSoundEffect(std::string const& _tag) const
{
	auto iter = sound_effect_map_.find(_tag);

	if (iter == sound_effect_map_.end())
		return sound_effect_dummy_;

	return iter->second;
}

std::unique_ptr<DirectX::SoundEffectInstance> const& K::AudioManager::FindSoundEffectInstance(std::string const& _tag) const
{
	auto iter = sound_effect_instance_map_.find(_tag);

	if (iter == sound_effect_instance_map_.end())
		return sound_effect_instance_dummy_;

	return iter->second;
}

void K::AudioManager::AddSoundEffectInstance(std::string const& _tag, std::unique_ptr<DirectX::SoundEffectInstance> _sound_effect_instance)
{
	sound_effect_instance_map_.insert(std::make_pair(_tag, std::move(_sound_effect_instance)));
}

void K::AudioManager::RemoveSoundEffectInstance(std::string const& _tag)
{
	auto iter = sound_effect_instance_map_.find(_tag);

	if (iter == sound_effect_instance_map_.end())
		return;

	sound_effect_instance_map_.erase(iter);
}

void K::AudioManager::_Finalize()
{
}

void K::AudioManager::_CreateSoundEffect(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	if (FindSoundEffect(_tag))
		throw std::exception{ "AudioManager::_CrateSoundEffect" };

	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "AudioManager::_CrateSoundEffect" };

	path_buffer /= _file_name;

	auto sound_effect = std::make_unique<DirectX::SoundEffect>(audio_engine_.get(), path_buffer.c_str());

	sound_effect_map_.insert(make_pair(_tag, std::move(sound_effect)));
}
