#pragma once
#ifndef GPGS_MANAGER_H
#define GPGS_MANAGER_H

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

#include <jni.h>
#include <errno.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include "gpg/gpg.h"
#include "JNIHelper.h"
#include "gpg/real_time_multiplayer_manager.h"
#include <cpu-features.h>
#include <sstream>
#include <algorithm>
#include <thread>
#include "cocos2d.h"

#define PAYLOAD_HEADER_LENGTH 1
#define PAYLOAD_HEADER_NEW_CONNECTION 'c'
#define PAYLOAD_HEADER_DISCONNECTED   'd'
#define PAYLOAD_HEADER_RENEW_CONNECTION 'r'
#define PAYLOAD_HEADER_GUEST_READY    'n'
#define PAYLOAD_HEADER_INTERMEDIATE_SCORE 'i'
#define PAYLOAD_HEADER_FINAL_SCORE 'f'

const int32_t MIN_PLAYERS = 1;
const int32_t MAX_PLAYERS = 7;

enum NEXT_PARTICIPANT {
    NEXT_PARTICIPANT_AUTOMATCH = -1,
    NEXT_PARTICIPANT_NONE = -2,
};

struct PLAYER_STATUS {
  std::string device_id_;
  std::string endpoint_id_;
  int32_t score_;
  bool connected_; // this node [endpoint_id_] is still online
  bool finished_;  // this node finished the game
  bool is_host_;   // I am hosting this link[I was advertising and this node was
                   // discovering
  bool is_direct_connection_;  // this node directly connects to me,
                               // not relayed to me by another device
};

class GPGSManager : public gpg::IRealTimeEventListener {
    
public:
    void InitServices(gpg::PlatformConfiguration &pc);
    void InitGoogleNearbyConnection(gpg::PlatformConfiguration &pc);
    static gpg::GameServices *GetGameServices();
    static void BeginUserInitiatedSignIn();
    static void SignOut();
    static void UnlockAchievement(const char *achievementId);
    static void IncrementAchievement(const char *achievementId, uint32_t steps = 1);
    static void SubmitHighScore(const char *leaderboardId, uint64_t score);
    static void ShowAchievements();
    static void ShowLeaderboard(const char *leaderboardId);
    static bool IsSignedIn() { return isSignedIn; }
    
    void QuickMatch();
    void InviteFriend();
    void ShowRoomInbox();
    static void ShowMatchInbox();
    void setPlayer1Name();
    void setPlayer2Name();

	std::string player1Name;
    std::string player2Name;
	std::vector<uint8_t> match_data;
    std::string match_string;
	
    static void ConfirmPendingCompletion();
    
    void PlayGame();
    void LeaveGame();
    static void TakeTurn(const bool winning, const bool losing);
    int32_t GetNextParticipant();
    static void ParseMatchData();
    static std::vector<uint8_t> SetupMatchData();
	
	static void EnableUI(bool enable);

	GPGSManager();
	~GPGSManager();
	
	// IRealTimeEventListener members
	virtual void OnRoomStatusChanged(gpg::RealTimeRoom const &room);

	virtual void OnParticipantStatusChanged(
		gpg::RealTimeRoom const &room,
		gpg::MultiplayerParticipant const &participant);

	virtual void OnDataReceived(
		gpg::RealTimeRoom const &room,
		gpg::MultiplayerParticipant const &from_participant,
		std::vector<uint8_t> data, bool is_reliable);

	// We are not using these callbacks below
	// because the app just waits for the room to become active,
	// no need to inspect individual changes
	virtual void OnConnectedSetChanged(gpg::RealTimeRoom const &room) {}

	virtual void OnP2PConnected(gpg::RealTimeRoom const &room,
                              gpg::MultiplayerParticipant const &participant) {}
	virtual void OnP2PDisconnected(
		gpg::RealTimeRoom const &room,
		gpg::MultiplayerParticipant const &participant) {}  
		
	gpg::RealTimeRoom room_;  // room status. This variable is updated each time
                        // the room status is updated
                        // in OnRoomStatusChanged()
    static std::unique_ptr<gpg::GameServices> gameServices;

	cocos2d::Vec2 gotDest; 
	bool gotIt;
private:
    static bool isSignedIn;
	
    std::unique_ptr<gpg::GameServices> service_; // gpg service instance
	std::unordered_map<std::string, PLAYER_STATUS> players_score_;  				// hashmap to
                                                                // keep
                                                                // tracking of
                                                                // player
                                                                // scores
    uint32_t nbc_state_;      // Nearby state
    int32_t score_counter_;   // Score counter of local player
    bool playing_;            // Am I playing a game?
    std::string self_id_;     // Local player's ID
    double start_time_;       // Game start time
    std::string service_id_;  // Nearby service id
    // synchronization primitive to synchronize
    // UIThread, Timer thread and gpg callback thread
    // mutable std::mutex mutex_;
    gpg::MessageListenerHelper msg_listener_;
  
    /*
    * nearby_connection_state are bit flags, they could co-exist.
    */
    enum nearby_connection_state {
    IDLE = 1,
    ADVERTISING = 2,
    DISCOVERING = 4,
    CONNECTED = 8,
    PLAYING = 16,
    FAILED = 32
    };

    std::unique_ptr<gpg::NearbyConnections> nearby_connection_;

    // Callbacks for GPG authentication.
    void OnAuthActionStarted(gpg::AuthOperation op);
    void OnAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status);

    // MessageListnerHelper to handle the messages
    void OnMessageDisconnectedCallback(int64_t receiver_id,
                                 std::string const &remote_endpoint);
    void OnMessageReceivedCallback(int64_t receiver_id,
                                 std::string const &remote_endpoint,
                                 std::vector<uint8_t> const &payload,
                                 bool is_reliable);
    void AddConnectionEndpoint(std::string const &remote_endpoint_id,
                                 bool is_native, bool is_host);
    void SendAllConnections(const std::string& accepting_endpoint_id);
    //void UpdatePlayerScore(std::string const & endpoint_id,
                               //  int score, bool final);
    bool DecodeScorePayload(std::vector<uint8_t> const &payload, int *p_score,
                                 const std::string &endpoint);
    //void UpdateScoreBoardUI(bool UIThreadRequired);
    void RemoveConnectionEndpoint(std::string const &remote_endpoint_id,
                                 bool need_broadcast = false);
    int32_t CountConnections(void);
};

#endif // GPGS_MANAGER_HPP
extern GPGSManager g_engine;
