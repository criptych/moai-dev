//
//  MOAIHusky.h
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#ifndef __libmoai__MOAIHusky__
#define __libmoai__MOAIHusky__

#include <dlfcn.h>
#include "Husky.h"

bool endsWith(const char *string, const char *ending);

class HuskyLoaderHandle {
public:
	HuskyLoaderHandle();
	HuskyLoaderHandle(void* dllhandle);
	void* dllhandle;
};

using LoaderHandleMap = std::map<std::string, HuskyLoaderHandle>;

class MOAIHusky : public MOAIGlobalClass <MOAIHusky, MOAILuaObject>, public HuskyObserver {
private:
	static int _getAvailable							( lua_State *L );
	static int _getCurrent							( lua_State *L );
	static int _setCurrent							( lua_State *L );
	static int _hasLeaderboards						( lua_State *L );
	static int _hasAchievements						( lua_State *L );
	static int _hasCloudSaves						( lua_State *L );
	static int _achievementReset						( lua_State *L );
	static int _achievementSet						( lua_State *L );
	static int _achievementSetCallback				( lua_State *L );
	static int _leaderboardUploadScore				( lua_State *L );
	static int _leaderboardSetScoreCallback			( lua_State *L );
	static int _leaderboardGetScores					( lua_State *L );
	static int _leaderboardSetGetScoresCallback		( lua_State *L );
	static int _cloudFileUpload						( lua_State *L );
	static int _cloudFileSetUploadCallback			( lua_State *L );
	static int _cloudFileDownload					( lua_State *L );
	static int _cloudFileSetDownloadCallback			( lua_State *L );
	static int _doTick								( lua_State *L );
	
	void HuskyObserverAchievementCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number);
	void HuskyObserverCloudFileDownloaded(const char *cloudfilename, const char *tempfile, bool success);
	void HuskyObserverCloudFileUploaded(const char *path, bool success);
	
	MOAILuaLocal _achievementCallback;
	MOAILuaLocal _leaderboardScoreSetCallback;
	MOAILuaLocal _leaderboardScoreGetCallback;
	MOAILuaLocal _cloudFileUploadCallback;
	MOAILuaLocal _cloudFileDownloadCallback;

	LoaderHandleMap *_map;
	void* _currentHuskyHandle;
	Husky* _instance;
	HuskyGetName* _fHuskyName;
	HuskyShutdownStaticInstance* _fHuskyShutdown;
	u16 _huskyCapabilities;
	
public:
	DECL_LUA_SINGLETON ( MOAIHusky )
	
							MOAIHusky	(  );
							~MOAIHusky	(  );
	//----------------------------------------------------------------//
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};



#endif /* defined(__libmoai__MOAIHusky__) */
