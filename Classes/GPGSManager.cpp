#include "GPGSManager.h"
#include "gpg/message_listener_helper.h"
#include "android/log.h"
#include "cocos2d.h"
#include "Scene1.h"
#include "Scene2.h"
#include <jni.h>
#define DEBUG_TAG "gpgslog"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))
GPGSManager g_engine;
bool GPGSManager::isSignedIn = false;
std::unique_ptr<gpg::GameServices> GPGSManager::gameServices;
//gpg::RealTimeRoom GPGSManager::room_;

GPGSManager::GPGSManager()
{
  gotIt=false;
}

GPGSManager::~GPGSManager()
{
	
}

/*
 * Got message from peers
 * room : The room which from_participant is in.
 * from_participant : The participant who sent the data.
 * data : The data which was recieved.
 * is_reliable : Whether the data was sent using the unreliable or
 *                    reliable mechanism.
 * In this app, packet format is defined as:
 * 1 byte: indicate score type 'F': final score 'U' updating score
 * 1 byte: score
 */
void GPGSManager::OnDataReceived(gpg::RealTimeRoom const &room,
                            gpg::MultiplayerParticipant const &from_participant,
                            std::vector<uint8_t> data, bool is_reliable) {
  if (data[0] == 'F' && is_reliable) {
    // Got final score
 //   players_score_[from_participant.Id()].score = data[1];
 //   players_score_[from_participant.Id()].finished = true;
    LOGI("Got final data from Dispname:%s ID:%s", from_participant.DisplayName().c_str(), from_participant.Id().c_str());
  } else if (data[0] == 'U' && !is_reliable) {
    // Got current score
  //  uint8_t score = players_score_[from_participant.Id()].score;
  //  players_score_[from_participant.Id()].score = std::max(score, data[1]);
    LOGI("Got data from Dispname:%s ID:%s", from_participant.DisplayName().c_str(), from_participant.Id().c_str());
   // Receiving coordinates
  } else if (data[0] == 'C' && !is_reliable) {
    int j=1;
    std::string fs;
    while(data[j]!='#')
    {
     fs+=data[j];
     j++;
    }
    j++;
    gotDest.x=std::atof(fs.c_str());
    fs.clear();
    while(j<data.size())
    {
     fs+=data[j];
     j++;
    }
    gotDest.y=std::atof(fs.c_str());
    CCLOG("Data_received %c = %c%c%c%c", data[0], data[1], data[2], data[3], data[4]);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("dispatchReceivedCoordinates");
  }
  //     UpdateScore();
}

/*
 * Room status change callback
 */
void GPGSManager::OnRoomStatusChanged(gpg::RealTimeRoom const &room) {
  if (room.Status() == gpg::RealTimeRoomStatus::ACTIVE) {
    // Room is set up, all players are connected, and we can start sending
    // messages between them.
  	room_ = room;
  }
}

/*
 * Invoked when participant status changed
 */
void GPGSManager::OnParticipantStatusChanged(
    gpg::RealTimeRoom const &room,
    gpg::MultiplayerParticipant const &participant) {

  // Update participant status
  LOGI("Participant %s status changed: %d", participant.Id().c_str(), participant.Status());

  if (participant.Status() != gpg::ParticipantStatus::JOINED) {
    {
//      std::lock_guard<std::mutex> lock(mutex_);
 //     if (players_score_.find(participant.Id()) != players_score_.end()) {
 //       players_score_[participant.Id()].finished = true;      }
    }
    //UpdateScore();
  }
}


/*
 * Callback: Authentication started
 */
 
void GPGSManager::OnAuthActionStarted(gpg::AuthOperation op) {
    LOGI("OnAuthActionStarted.");
    switch ( op ) {
        case gpg::AuthOperation::SIGN_IN:
            LOGI("Signing in to GPG.");
            break;
        case gpg::AuthOperation::SIGN_OUT:
            LOGI("Signing out from GPG.");
            break;
    }
}

/*
 * Callback: Authentication finishes
 */

 void GPGSManager::OnAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status) {
    LOGI("OnAuthActionFinished.");
    if (status == gpg::AuthStatus::VALID){
        LOGI("Signing In.");
      /*if (gpg::IsSuccess(status)) {
        LOGI("FetchSelf........");
        service_->Players().FetchSelf([this](
            gpg::PlayerManager::FetchSelfResponse const &response) {
          if (gpg::IsSuccess(response.status)) {
            self_id_ = response.data.Id();
          }
        });
      }*/
    }
    else{
        LOGI("Signing Out.");
    }
}

/*
 * Get GameServices
 */
gpg::GameServices *GPGSManager::GetGameServices() {
    return gameServices.get();
}

/*
 * Start Authorization
 */
void GPGSManager::BeginUserInitiatedSignIn() {
    if (!gameServices->IsAuthorized()) {
        LOGI("StartAuthorizationUI.");
        gameServices->StartAuthorizationUI();
    }
}

/*
 * Sign Out
 */
void GPGSManager::SignOut() {
    if (gameServices->IsAuthorized()) {
        LOGI("SignOut.");
        gameServices->SignOut();
    }
}

/*
 * Unlock specified achievement
 */
void GPGSManager::UnlockAchievement(const char *achievementId) {
    if (gameServices->IsAuthorized()) {
        LOGI("Achievement unlocked.");
        gameServices->Achievements().Unlock(achievementId);
    }
}

/*
 * Increase specified achievemet
 */
void GPGSManager::IncrementAchievement(const char *achievementId, uint32_t steps/* = 1*/)
{
    if (gameServices->IsAuthorized()) {
        LOGI("Achievement Increase");
        gameServices->Achievements().Increment(achievementId, steps);
    }
}

/*
 * Submit high score
 */
void GPGSManager::SubmitHighScore(const char *leaderboardId, uint64_t score) {
    if (gameServices->IsAuthorized()) {
        LOGI("High score submitted");
        gameServices->Leaderboards().SubmitScore(leaderboardId, score);
    }
}

/*
 * Show achievemets
 */
void GPGSManager::ShowAchievements()
{
    if (gameServices->IsAuthorized()) {
        LOGI("Show achievement");
        //gameServices->Achievements().ShowAllUI();
    }
}

/*
 * Show Leaderboard
 */
void GPGSManager::ShowLeaderboard(const char *leaderboardId)
{
    if (gameServices->IsAuthorized()) {
        LOGI("Show achievement");
        //gameServices->Leaderboards().ShowUI(leaderboardId);
    }
}

/*
 * Initialize GooglePlayGameServices
 */
 
void GPGSManager::InitServices(gpg::PlatformConfiguration &pc)
{
    LOGI("Initializing Services using GPGSManager");
    if (!gameServices) {
        LOGI("Game Services have not been initialized, creating new Game Services");
        // Game Services have not been initialized, create a new Game Services.
        gameServices = gpg::GameServices::Builder()
        /*.SetLogging(gpg::DEFAULT_ON_LOG, gpg::LogLevel::VERBOSE)*/
        .SetDefaultOnLog(gpg::LogLevel::VERBOSE)
        .SetOnAuthActionStarted([this](gpg::AuthOperation op){
            LOGI("Sign in started");
            OnAuthActionStarted(op);
        })
        .SetOnAuthActionFinished([this](gpg::AuthOperation op, gpg::AuthStatus status){
            LOGI("Sign in finished");
            if( status == gpg::AuthStatus::VALID) {
            LOGI("AuthStatus VALID");
                isSignedIn = true;
            } else {
            LOGI("AuthStatus INVALID");
                isSignedIn = false;
            }
            OnAuthActionFinished( op, status);
        })
        .SetOnMultiplayerInvitationEvent([this](gpg::MultiplayerEvent event, std::string match_id,  
               gpg::MultiplayerInvitation invitation) {
             // Invoked when invitation has been received
             // It can be received from the Play Game app, a notification, or
             // live while the app is running.
             // (e.g. Though PlayGam app, while the app is running)
             LOGI("MultiplayerInvitationEvent callback");

             if (event == gpg::TurnBasedMultiplayerEvent::UPDATED_FROM_APP_LAUNCH) {
               // In this case, an invitation has been accepted already
               // in notification or in Play game app
               gpg::RealTimeMultiplayerManager::RealTimeRoomResponse result =
                   gameServices->RealTimeMultiplayer().AcceptInvitationBlocking(
                       invitation, this/*IRealTimeEventListener*/);
               if (gpg::IsSuccess(result.status)) {
                 room_ = result.room;
                 gameServices->RealTimeMultiplayer().ShowWaitingRoomUI(
                     room_, MIN_PLAYERS,
                     [this](gpg::RealTimeMultiplayerManager::   
                                WaitingRoomUIResponse const &waitResult) {
                       EnableUI(true);
                       if (gpg::IsSuccess(waitResult.status)) {
                         PlayGame();
                       }
                     });
               } else {
                 LeaveGame();
               }
             } else {
               // Otherwise, show default inbox and let players to accept an
                // invitation
             ShowRoomInbox();
			       auto scene = Scene1::createScene();
	           cocos2d::Director::getInstance()->pushScene(scene);
             }
           })
        .Create(pc);
    }
    LOGI("InitServices Created");
    //InitGoogleNearbyConnection(pc);
}

/*
 * InitGoogleNearbyConnection():
 *    Create an NearbyConnection interface, update our internal indicators
 *    according to connection creation result
 */
/*void GPGSManager::InitGoogleNearbyConnection(gpg::PlatformConfiguration &pc) {
  // Need only one connection
  if (nearby_connection_ != nullptr) {
    return;
  }
  nbc_state_ = nearby_connection_state::IDLE;
 
  ndk_helper::JNIHelper &helper = *ndk_helper::JNIHelper::GetInstance();
  //helper.AttachCurrentThread();

  // Two devices can connect using Nearby Connections if they share the
  // same service ID.  This value is set in AndroidManifest.xml, it can be any
  // string that is unique for your application
  service_id_ = helper.GetNearbyConnectionServiceID();
  // Prepare to create a nearby connection interface
  gpg::NearbyConnections::Builder nbBuilder;
  nearby_connection_ = nbBuilder.SetDefaultOnLog(gpg::LogLevel::VERBOSE)
      // register callback so we get notified the nearby connect is ready
      .SetOnInitializationFinished([this](gpg::InitializationStatus status) {
        switch (status) {
          case gpg::InitializationStatus::VALID:
            // Our interface is ready to use [ could advertise or discover ]
            LOGI("InitializationFinished() returned VALID");
            nbc_state_ = nearby_connection_state::IDLE;
             // Configure our listener to use for listening
            msg_listener_.SetOnMessageReceivedCallback(
                              [this](int64_t receiver_id,
                                     std::string const &remote_endpoint,
                                     std::vector<uint8_t> const &payload,
                                     bool is_reliable) {
                                OnMessageReceivedCallback(receiver_id,
                                                          remote_endpoint,
                                                          payload, is_reliable);
                              })
                .SetOnDisconnectedCallback([this](
                    int64_t receiver_id, std::string const &remote_endpoint) {
                  OnMessageDisconnectedCallback(receiver_id, remote_endpoint);
                });
            LOGI("Nearby Connection Interface is ready to use!");
            break;
          case gpg::InitializationStatus::ERROR_VERSION_UPDATE_REQUIRED:
            /*
             * interface need update, we should prompt user to do so, or we do
             * it automatically here [not done]
             * /
            LOGI(
                "InitializationFinished(): ERROR_VERSION_UPDATE_REQUIRED "
                "returned,"
                "please restart your device to update Google play service");
            nbc_state_ = nearby_connection_state::FAILED;
            break;
          case gpg::InitializationStatus::ERROR_INTERNAL:
            /*
             * Error happened, interface is not ready to use
             * /
            LOGI(
                "InitializationFinished() failed, unable to start nearby connection");
            nbc_state_ = nearby_connection_state::FAILED;
            EnableUI(false);
            break;
          default:
            LOGI("InitializationFinished(): Unrecognized error code: %d",
                 (int)status);
            nbc_state_ = nearby_connection_state::FAILED;
            EnableUI(false);
            break;
        }
      })
      .Create(pc);

  // At this point, connection is still not useful; wait till
  // InitializationFinished()
  // telling us that VALID initialization has been completed
  LOGI("InitGoogleNearbyConnection() created interface: %p",
       nearby_connection_.get());
}*/

void GPGSManager::LeaveGame() {
  gameServices->RealTimeMultiplayer().LeaveRoom(room_, [](gpg::ResponseStatus const &response) {
    if (IsSuccess(response)) {
      // We left successfully. We can now join another room.
      LOGI("Left room");
    }
  });
}

/*
 * Show room inbox
 */
void GPGSManager::ShowRoomInbox() {
  gameServices->RealTimeMultiplayer().ShowRoomInboxUI([this](
      gpg::RealTimeMultiplayerManager::RoomInboxUIResponse const &response) {
    if (gpg::IsSuccess(response.status)) {
      gpg::RealTimeMultiplayerManager::RealTimeRoomResponse result =
          gameServices->RealTimeMultiplayer().AcceptInvitationBlocking(
              response.invitation, this);
      if (gpg::IsSuccess(result.status)) {
        room_ = result.room;
        gameServices->RealTimeMultiplayer().ShowWaitingRoomUI(
            room_, MIN_PLAYERS,
            [this](
                gpg::RealTimeMultiplayerManager::WaitingRoomUIResponse const &
                    waitResult) {
              EnableUI(true);
              if (gpg::IsSuccess(waitResult.status)) {
                PlayGame();
              }
            });
      } else EnableUI(true);  // Go back to original state for retry
    } else {
      LOGI("Invalid response status");
      EnableUI(true);  // Go back to original state for retry
    }
  });
  EnableUI(false);
}

/*
 * Quick match
 * - Create a match with minimal setting and play the game
 */
void GPGSManager::QuickMatch()
{
    gpg::RealTimeMultiplayerManager& manager = gameServices->RealTimeMultiplayer();
    gpg::RealTimeRoomConfig config = gpg::RealTimeRoomConfig::Builder()
    .SetMinimumAutomatchingPlayers(MIN_PLAYERS)
    .SetMaximumAutomatchingPlayers(MAX_PLAYERS)
    .Create();
    
    // Create new match with the config
    manager.CreateRealTimeRoom(config, this, [this](gpg::RealTimeMultiplayerManager::RealTimeRoomResponse const &matchResponse)
                                {
                   if (matchResponse.status == gpg::MultiplayerStatus::VALID) {
                     LOGI("QuickMatch game success.");
										 PlayGame();
										 gameServices->RealTimeMultiplayer().ShowWaitingRoomUI(
										 matchResponse.room,
										 MIN_PLAYERS,
										 [this](gpg::RealTimeMultiplayerManager::WaitingRoomUIResponse const & wait_response) {
										 EnableUI(true);
										 if (IsSuccess(wait_response.status)) {
										 // The room is set up. Proceed with gameplay.
                      LOGI("Room is set up.");
										  }
										});
										EnableUI(true);
									} else {
                    LOGI("QuickMatchs game failed");
										EnableUI(true);
                    }               								 
								});
EnableUI(false);
}
/*
 * Invite friends
 * - Show Player Select UI via ShowPlayerSelectUI,
 * - When the UI is finished, create match and show game UI
 */
void GPGSManager::InviteFriend()
{
  LOGI("InviteFriend Start");

  gameServices->RealTimeMultiplayer().ShowPlayerSelectUI(MIN_PLAYERS, MAX_PLAYERS, true,
              [this](gpg::RealTimeMultiplayerManager::PlayerSelectUIResponse const & response) {
    LOGI("Inviting friends");
                
    if (gpg::IsSuccess(response.status)) {
      LOGI("response status is VALID");
      
      // Create new match with the config
      gpg::RealTimeRoomConfig config = gpg::RealTimeRoomConfig::Builder()
      .PopulateFromPlayerSelectUIResponse(response)
		  .Create();
               
      gameServices->RealTimeMultiplayer().CreateRealTimeRoom(config, this, [this](gpg::RealTimeMultiplayerManager::RealTimeRoomResponse const & matchResponse) {

        if (gpg::MultiplayerStatus::VALID == matchResponse.status) {
          LOGI("Room created");
         
          //PlayGame();
          room_ = matchResponse.room;
          
          //setPlayer1Name();
          
          gameServices->RealTimeMultiplayer().ShowWaitingRoomUI(
          room_, MIN_PLAYERS,
          [this](gpg::RealTimeMultiplayerManager::WaitingRoomUIResponse const &waitResult) {
            EnableUI(true);
            
            if (gpg::IsSuccess(waitResult.status)) {
              PlayGame();
            }
          });
        } else if (gpg::MultiplayerStatus::VALID_BUT_STALE == matchResponse.status) {
          LOGI("VALID_BUT_STALE");
          EnableUI(true);
        } else if (gpg::MultiplayerStatus::ERROR_INTERNAL == matchResponse.status) {
          LOGI("ERROR_INTERNAL, error code: %d", (int)matchResponse.status);
          EnableUI(true);
        } else {
          LOGI("Room failed");
          EnableUI(true); 
        }
      });
    }
  EnableUI(false);
  });
}

/*
 * Retrieve next participant index in the match
 * returns NEXT_PARTICIPANT_AUTOMATCH if automatching
 * returns NEXT_PARTICIPANT_NONE if there is no next participant candidate
 */
int32_t GPGSManager::GetNextParticipant() {
    gpg::PlayerManager::FetchSelfResponse localPlayer =
    gameServices->Players().FetchSelfBlocking();
    
    //Retrieve next participant
    std::vector<gpg::MultiplayerParticipant> participants =
    room_.Participants();
    int32_t localPlayerIndex = -1;
    int32_t nextPlayerIndex = -1;
    int32_t size = participants.size();
    
    LOGI("# of participants: %d", size);
    for (int32_t i = 0; i < size; ++i) {
        if (participants[i].Player().Id().compare(localPlayer.data.Id()) == 0) {
            localPlayerIndex = i;
        }
        LOGI("participant: %s", participants[i].Player().Id().c_str());
    }
    if (localPlayerIndex == -1) {
        LOGI("Local player not found in a match?");
        return -1;
    }
    
    for (int32_t i = 1; i < size; ++i) {
        int32_t index = (localPlayerIndex + i) % size;
        if (participants[index].Status() == gpg::ParticipantStatus::INVITED ||
            participants[index].Status() == gpg::ParticipantStatus::JOINED ||
            participants[index].Status() == gpg::ParticipantStatus::NOT_INVITED_YET) {
            LOGI("Found next participant");
            nextPlayerIndex = index;
        }
    }
    if (nextPlayerIndex == -1) {
        // No next participant found
        // Do we have Auto-match player?
        if (room_.RemainingAutomatchingSlots()) {
            LOGI("Auto matching for next participant");
            return NEXT_PARTICIPANT_AUTOMATCH;
        } else
            return NEXT_PARTICIPANT_NONE;
    }
    return nextPlayerIndex;
}

/*
 * OnMessageReceived(received_id, remote_endpoint, payload, is_reliable)
 *    for our case, we only looking for a score and update our cached value
 *    otherwise we ignore
 */
void GPGSManager::OnMessageReceivedCallback(int64_t receiver_id,
                                       std::string const &remote_endpoint,
                                       std::vector<uint8_t> const &payload,
                                       bool is_reliable) {
  std::string msg;
  for (auto ch : payload) msg += ch;

  switch (msg[0]) {
    case PAYLOAD_HEADER_NEW_CONNECTION: {
      LOGI("Adding relayed endpoint (%s, %d) to me",
           remote_endpoint.c_str(), static_cast<int>(remote_endpoint.length()));
      std::string other_endpoint = msg.substr(1);
      AddConnectionEndpoint(other_endpoint, false, false);
      EnableUI(true);
      return;
    }
    case PAYLOAD_HEADER_GUEST_READY: {
      // The link I am hosting is up, and the other end ( guest end )
      // is ready to accept all of my connected nodes
      LOGI("Received ready message from =%s", remote_endpoint.c_str());
      SendAllConnections(remote_endpoint);
      return;
    }
    case PAYLOAD_HEADER_INTERMEDIATE_SCORE: {
      int score;
      if (!DecodeScorePayload(payload, &score, remote_endpoint)) {
        LOGI("DecodeScorePayload return failed\n");
      } else {
      //  UpdatePlayerScore(remote_endpoint, score, false);
       // UpdateScoreBoardUI(true);
      }
      return;
    }
    case PAYLOAD_HEADER_FINAL_SCORE: {
      int score;
      if (!DecodeScorePayload(payload, &score,remote_endpoint)) {
        LOGI("DecodeScorePayload return failed\n");
      } else {
       // UpdatePlayerScore(remote_endpoint, score, true);
        //UpdateScoreBoardUI(true);
      }
      return;
    }
    case PAYLOAD_HEADER_DISCONNECTED: {
      LOGI("Received Disconnect Notification for Endpoint ID %s",
           remote_endpoint.c_str());
      RemoveConnectionEndpoint(remote_endpoint, false);
      EnableUI(true);
      return;
    }
    default: {
      // Drop the message
      LOGI("Unknown payload type: from(%s) with payload(%s) in %s @ line %d",
           remote_endpoint.c_str(), msg.c_str(), __FILE__, __LINE__);
      return;
    }
  }
}

/*
 * OnMessageDisconnectedCallback(): update our player database
 */
void GPGSManager::OnMessageDisconnectedCallback(int64_t receiver_id,
                                           std::string const &remote_endpoint) {
  RemoveConnectionEndpoint(remote_endpoint, true);
  if (CountConnections() == 0) {
    nbc_state_ &= ~nearby_connection_state::CONNECTED;
    if (nbc_state_ == 0) {
      nbc_state_ |= nearby_connection_state::IDLE;
    }
  }
  EnableUI(true);
}


/*
 * AddConnectionEndpoint(): add this to the player list if it is not in
 */
void GPGSManager::AddConnectionEndpoint(std::string const &remote_endpoint_id,
                                   bool is_native, bool is_host) {
  // Find if this one is in
  auto it = players_score_.find(remote_endpoint_id);
  if (it == players_score_.end()) {
    PLAYER_STATUS player_info;
    memset(&player_info, 0, sizeof(player_info));
    player_info.connected_ = true;
    player_info.is_direct_connection_ = is_native;
    player_info.is_host_ = is_host;
    player_info.endpoint_id_ = remote_endpoint_id;
    players_score_[remote_endpoint_id] = player_info;
  } else {
    if (it->second.connected_ == true) {
      LOGE("accepting a connection while we are connected in %s @ %d", __FILE__,
           __LINE__);
    }
    it->second.finished_ = false;
    it->second.connected_ = true;
    it->second.is_direct_connection_ = is_native;
    it->second.is_host_ = is_host;
  }
}

/*
 * RemoveConnectionEndpoint(): turn it into NOT connected
 */
void GPGSManager::RemoveConnectionEndpoint(std::string const &remote_endpoint_id,
                                      bool need_broadcast) {
  auto it = players_score_.find(remote_endpoint_id);
  if (it != players_score_.end()) {
    it->second.finished_ = true;
    it->second.connected_ = false;
  }

  if (need_broadcast && it->second.is_host_) {
    // build up array of the connected endpoints to me DIRECTLY
    std::vector<std::string> endpoints;
    for (it = players_score_.begin(); it != players_score_.end(); ++it) {
      if (it->second.connected_ && it->second.is_direct_connection_ &&
          it->second.is_host_ &&
          it->first != remote_endpoint_id) {
        endpoints.push_back(it->first);
      }
    }
    if (endpoints.size()) {
      std::vector<uint8_t> payload;
      payload.resize(PAYLOAD_HEADER_LENGTH + remote_endpoint_id.size());
      payload[0] = (uint8_t)PAYLOAD_HEADER_DISCONNECTED;
      memcpy(&(payload[1]), remote_endpoint_id.c_str(),
             remote_endpoint_id.size());

      nearby_connection_->SendReliableMessage(endpoints, payload);
    }
  }
}

/*
 * CountConnections(void): report current valid connections
 */
int32_t GPGSManager::CountConnections(void) {
  int32_t count = 0;
  for (auto it = players_score_.begin(); it != players_score_.end(); ++it) {
    if (it->second.connected_) {
      count++;
    }
  }
  return count;
}

/*
 * DecodeScorePayload()
 */
bool GPGSManager::DecodeScorePayload(std::vector<uint8_t> const &payload, int *p_score,
                        const std::string &endpoint) {
    if(!p_score) {
        LOGE("null pointer for decideScorePayload %p", p_score);
        return false;
    }
    std::string payload_str(reinterpret_cast<const char*>(&payload[0]),payload.size());
    int endpoint_size = (payload_str[1] - '0') * 10 + (payload_str[2] - '0');

    //get our score
    payload_str = payload_str.substr(3+endpoint_size);
    *p_score = 0;
    for (auto ch : payload_str) {
        if(ch < '0' || ch > '9')
            break;
        *p_score = (*p_score) * 10 + (ch - '0');
    }

    return true;
}

/*
 * SendAllConnectios():
 *    Send all of my directly connected endpoints to the given endpoint
 *    because it just established a connection to this device and has no
 *    knowledge of other endpoints [directly connecting to me ]
 */
void GPGSManager::SendAllConnections(const std::string& accepting_endpoint_id) {
  std::vector<uint8_t> payload;
  for (auto it = players_score_.begin(); it != players_score_.end(); ++it) {
    if (it->second.endpoint_id_ == accepting_endpoint_id ||
        !it->second.is_direct_connection_)
      continue;
    payload.resize(it->second.endpoint_id_.length() + 1);
    payload[0] = static_cast<uint8_t>(PAYLOAD_HEADER_NEW_CONNECTION);
    strncpy(reinterpret_cast<char *>(&payload[1]),
            it->second.endpoint_id_.c_str(),
            it->second.endpoint_id_.length());
    LOGI("Sending connection to %s for %s", accepting_endpoint_id.c_str(),
         it->second.endpoint_id_.c_str());
    nearby_connection_->SendReliableMessage(accepting_endpoint_id, payload);
  }
}

/*
 * Enable/Disable management UI
 */
void GPGSManager::EnableUI(bool enable) {
  
}

void GPGSManager::PlayGame() {
    CCLOG("Level");
	
   auto scene = Scene1::createScene();
   cocos2d::Director::getInstance()->replaceScene(scene);
}

void GPGSManager::setPlayer1Name()
{
    gpg::PlayerManager::FetchSelfResponse localPlayer = gameServices->Players().FetchSelfBlocking();
    player1Name = localPlayer.data.Name();
    LOGI("player1 name:%s", localPlayer.data.Name().c_str());
}

void GPGSManager::setPlayer2Name()
{
    gpg::PlayerManager::FetchSelfResponse localPlayer = gameServices->Players().FetchSelfBlocking();
    player2Name = localPlayer.data.Name();
    LOGI("player2 name:%s", localPlayer.data.Name().c_str());
}


extern "C" JNIEXPORT jint GPG_JNI_OnLoad(JavaVM *vm, void *reserved) {


  gpg::AndroidInitialization::JNI_OnLoad(vm);
  
  return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_fpl_fplbase_FPLActivity_nativeOnActivityResult(
    JNIEnv *env, jobject thiz, jobject activity, jint request_code,
    jint result_code, jobject data) {
  gpg::AndroidSupport::OnActivityResult(env, activity, request_code,
                                        result_code, data);
  //fplbase::LogInfo(fplbase::kApplication, "GPG: nativeOnActivityResult");
}