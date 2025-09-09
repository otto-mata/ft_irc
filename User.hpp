#ifndef USER_HPP
#define USER_HPP
#include "Common.hpp"

namespace Core {

class User
{
private:
  int _fd;
  std::string _nickname;
  std::string _hostname;
  std::string _incomingBuffer;
  std::string _outgoingBuffer;
  std::string _username;
  std::string _realname;
  std::string _ipAdress;
  bool _isRegistered;
  bool _hasNick;
  bool _hasUser;
  bool _hasPass;
  bool _hasSentPassword;
  bool _isValidPassword;
  bool _disconnected;
  bool _capabilitiesNegotiationFinished;

public:
  User(int fd);
  ~User(void);
  bool ReadyToSend(void);
  int Fileno(void);

  bool FullyRegistered(void);

  void SetIncomingBuffer(const std::string& from);
  const std::string& GetIncomingBuffer(void);
  
  std::string FullIdentityString(void);
  
  void AppendToOutgoingBuffer(const std::string& from);
  void AppendToOutgoingBuffer(const char* from);
  void SetOutgoingBuffer(const std::string& from);
  void SetOutgoingBuffer(const char* from);
  const std::string& GetOutgoingBuffer(void);

  void ClearOutgoingBuffer(void);
  void ClearIncomingBuffer(void);

  void SetNickname(const std::string& from);
  const std::string& GetNickname(void);

  void SetUsername(const std::string& from);
  const std::string& GetUsername(void);

  void SetRealName(const std::string& from);
  const std::string& GetRealName(void);

  void FinishCapabilitiesNegotiation(void);
  void PendingCapabilitiesNegotiation(void);
  bool HasFinishedCapNeg(void);

  void CompletedRegistrationRoutine(const std::string& from);
  
  void SetPasswordReceived(bool);
  void SetPasswordValid(bool);
  bool HasSentValidPassword(void);
  
  void ResolveHostname(void);
  const std::string& GetHostname(void);
};

} // namespace Core

#endif
