#ifndef USER_HPP
#define USER_HPP
#include <Common.hpp>
#include <Logger.hpp>

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
  std::string _quitMessage;
  Logging::Engine log;
  unsigned short _remotePort;
  bool _hasNick;
  bool _hasUser;
  bool _hasSentPassword;
  bool _isValidPassword;
  bool _disconnected;
  bool _capabilitiesNegotiationFinished;
  bool _toDelete;

public:
  User(int fd);
  ~User(void);
  bool ReadyToSend(void);
  int Fileno(void);

  bool FullyRegistered(void);

  void SetIncomingBuffer(const std::string& from);
  const std::string& GetIncomingBuffer(void);
  std::string GetEscapedIncomingBuffer(void);

  std::string FullIdentityString(void);

  void AppendToOutgoingBuffer(const std::string& from);
  void AppendToOutgoingBuffer(const char* from);
  void SetOutgoingBuffer(const std::string& from);
  void SetOutgoingBuffer(const char* from);
  const std::string& GetOutgoingBuffer(void);
  std::string GetEscapedOutgoingBuffer(void);

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

  bool CompletedRegistrationRoutine(const std::string& from);

  void SetPasswordReceived(bool);
  bool SetPasswordValid(bool);
  bool HasSentValidPassword(void);

  void ResolveHostname(void);
  const std::string& GetHostname(void);
  std::string RemoteConnectionString(void);

  void SetQuitMessage(const std::string& msg);
  const std::string& GetQuitMessage(void);
  void MarkForDeletion(void);
  bool MustBeDeleted(void);
};

} // namespace Core

#endif
