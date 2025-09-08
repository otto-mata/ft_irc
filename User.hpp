#ifndef USER_HPP
#define USER_HPP
#include "Common.hpp"

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

public:
  User(int fd);
  ~User(void);
  bool ReadyToSend(void); 
  int Fileno(void);

  bool FullyRegistered(void);
  
  void SetIncomingBuffer(const std::string& from);
  const std::string& GetIncomingBuffer(void);
  
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

};

#endif
