#include "SceneManager.h"

SceneD* SceneManager::_CurrentScene = nullptr;
std::unordered_map<std::string, SceneD*> SceneManager::_KnownScenes;
entt::registry SceneManager::Prefabs;

SceneD* SceneManager::Current() {
	return _CurrentScene;
}
bool SceneManager::HasScene(const std::string& name) {
	auto it = _KnownScenes.find(name);
	if (it != _KnownScenes.end()) {
		return true;
	}
	return false;
}
SceneD* SceneManager::Get(const std::string& name) {
	if (!HasScene(name))
		return nullptr;
	else
		return _KnownScenes[name];
}

bool SceneManager::SetCurrentScene(const std::string& name) {
	auto it = _KnownScenes.find(name);
	if (it != _KnownScenes.end()) {
		if (_CurrentScene != it->second) {
			if (_CurrentScene != nullptr)
				//_CurrentScene->OnClose();
			_CurrentScene = it->second;
			//_CurrentScene->OnOpen();
		}
		return true;
	}
	return false;
}

void SceneManager::RegisterScene(const std::string& name, SceneD* scene) {
	//LOG_ASSERT(!HasScene(name), "A scene with that name already exists!");
	if (scene == nullptr)
		//scene = new Scene();
	_KnownScenes[name] = scene;
}

SceneManager::SceneIterator SceneManager::Each() {
	return SceneIterator();
}

void SceneManager::DestroyScenes() {
	for (auto& kvp : _KnownScenes) {
		delete kvp.second;
	}
	_KnownScenes.clear();
	_CurrentScene = nullptr;
}