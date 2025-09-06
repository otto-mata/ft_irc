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

  void SetIncomingBuffer(const std::string& from);
  const std::string& GetIncomingBuffer(void);
  
  void SetOutgoingBuffer(const std::string& from);
  const std::string& GetOutgoingBuffer(void);

};

#endif
