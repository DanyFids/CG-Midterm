#pragma once
#include "Scene.h"
#include <unordered_map>
class SceneManager {
public:
	struct SceneIterator {
		typedef std::unordered_map<std::string, SceneD*>::iterator iterator;
		typedef std::unordered_map<std::string, SceneD*>::const_iterator const_iterator;
		iterator begin() { return SceneManager::_KnownScenes.begin(); }
		iterator end() { return SceneManager::_KnownScenes.end(); }
		const_iterator begin() const { return SceneManager::_KnownScenes.begin(); }
		const_iterator end() const { return SceneManager::_KnownScenes.end(); }
	};
	static SceneD* Current();
	static bool SetCurrentScene(const std::string& name);
	static bool HasScene(const std::string& name);
	static SceneD* Get(const std::string& name);

	static void RegisterScene(const std::string& name, SceneD* scene = nullptr);
	static SceneIterator Each();
	static void DestroyScenes();
	static entt::registry Prefabs;
private:
	static SceneD* _CurrentScene;
	static std::unordered_map<std::string, SceneD*> _KnownScenes;
};

// We can make some macros to shorten our calls
#define CurrentScene() SceneManager::Current()
#define CurrentRegistry() SceneManager::Current()->Registry()
#define GetRegistry(name) SceneManager::Get(name)->Registry()