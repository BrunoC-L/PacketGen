

 /*************************************************************\ 
 \                                                             / 
 /   THIS FILE WAS AUTOMATICALLY GENERATED USING PACKET-GEN    \ 
 \                                                             / 
 /    DO NOT MODIFY THIS FILE, SEE PACKET-GEN INSTRUCTIONS     \ 
 \            FOR UPDATING THE PACKET DEFINITIONS              / 
 /                                                             \ 
 \*************************************************************/ 


#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <functional>

namespace NS_Server {

class LoginC2S {
public:
    constexpr static uint8_t type = 4;
    	uint8_t mutable _length_username;
	uint8_t mutable _length_password;
    std::string username;
	std::string password;
    LoginC2S() = default;
    LoginC2S(std::string&&  username,std::string&&  password)  : username(std::move(username)),password(std::move(password)),_length_username(username.length()),_length_password(password.length()) {}
    LoginC2S(std::istream& _STREAM, const int _STREAM_LENGTH) {
        if (_STREAM.tellg().operator+(2) > _STREAM_LENGTH) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(2)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
		}
		_STREAM.read((char*)this, 2);
        {
			uint8_t L = _length_username;
			if (_STREAM.tellg().operator+(L) > _STREAM_LENGTH) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(L)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
			}
			char* temp = new char[L + 1];
			_STREAM.read(temp, L);
			temp[L] = '\0';
			username = temp;
		delete[] temp;
		}
{
			uint8_t L = _length_password;
			if (_STREAM.tellg().operator+(L) > _STREAM_LENGTH) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(L)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
			}
			char* temp = new char[L + 1];
			_STREAM.read(temp, L);
			temp[L] = '\0';
			password = temp;
		delete[] temp;
		}
    }
};

class LoginConfirmS2C {
public:
    constexpr static uint8_t type = 5;
    LoginConfirmS2C()  = default;
    template <bool writeType>
    uint16_t write(std::ostream& _STREAM) const {
        uint16_t _tellp_beg;
        if constexpr (writeType) {
            _tellp_beg = _STREAM.tellp();
            _STREAM.seekp(2); // 2 bytes for size
            _STREAM.write((char*)&type, 1);
        }
		_STREAM.write((char*)this, 0);
        if constexpr (writeType) {
            uint16_t size = _STREAM.tellp().operator-(2 + _tellp_beg); // remove 2 bytes storing the size itself
            _STREAM.seekp(_tellp_beg);
            _STREAM.write((char*)&size, 2);
            return size + 2; // total length includes 2 bytes for size
        }
        return -1;
    }
};

class PlayerPosition {
public:
    constexpr static uint8_t type = 1;
    uint16_t x;
	uint16_t y;
	uint8_t z;
    PlayerPosition() = default;
    PlayerPosition(uint16_t x,uint16_t y,uint8_t z)  : x(x),y(y),z(z) {}
    PlayerPosition(std::istream& _STREAM, const int _STREAM_LENGTH) {
        if (_STREAM.tellg().operator+(5) > _STREAM_LENGTH) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(5)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
		}
		_STREAM.read((char*)this, 5);
    }
    template <bool writeType>
    uint16_t write(std::ostream& _STREAM) const {
        uint16_t _tellp_beg;
        if constexpr (writeType) {
            _tellp_beg = _STREAM.tellp();
            _STREAM.seekp(2); // 2 bytes for size
            _STREAM.write((char*)&type, 1);
        }
		_STREAM.write((char*)this, 5);
        if constexpr (writeType) {
            uint16_t size = _STREAM.tellp().operator-(2 + _tellp_beg); // remove 2 bytes storing the size itself
            _STREAM.seekp(_tellp_beg);
            _STREAM.write((char*)&size, 2);
            return size + 2; // total length includes 2 bytes for size
        }
        return -1;
    }
};

class Player {
public:
    constexpr static uint8_t type = 2;
    uint16_t id;
	uint8_t mutable _length_friends;
	uint8_t mutable _length_name;
    std::string name;
    std::vector<std::string> friends;
    PlayerPosition pos;
    Player() = default;
    Player(uint16_t id,std::string&&  name,PlayerPosition&&  pos)  : id(id),name(std::move(name)),pos(std::move(pos)),_length_name(name.length()) {}
    Player(std::istream& _STREAM, const int _STREAM_LENGTH) {
        if (_STREAM.tellg().operator+(4) > _STREAM_LENGTH) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(4)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
		}
		_STREAM.read((char*)this, 4);
        {
			uint8_t L = _length_name;
			if (_STREAM.tellg().operator+(L) > _STREAM_LENGTH) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(L)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
			}
			char* temp = new char[L + 1];
			_STREAM.read(temp, L);
			temp[L] = '\0';
			name = temp;
		delete[] temp;
		}
        friends.reserve(_length_friends);
        for (int i = 0; i < _length_friends; ++i) {
            uint8_t LEN;
            if (_STREAM.tellg().operator+(1) > _STREAM_LENGTH) {
                std::string ERRMSG = std::to_string(_STREAM.tellg()) + ",1," + std::to_string(_STREAM_LENGTH);
                throw std::out_of_range(ERRMSG);
            }
            _STREAM.read((char*)&LEN, 1);
            if (_STREAM.tellg().operator+(LEN) > _STREAM_LENGTH) {
                std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(LEN)+ "," + std::to_string(_STREAM_LENGTH);
                throw std::out_of_range(ERRMSG);
            }
            char* temp = new char[LEN + 1];
            _STREAM.read(temp, LEN);
            temp[LEN] = '\0';
            friends.emplace_back(temp);
            delete[] temp;
        }
        pos = std::move(PlayerPosition(_STREAM, _STREAM_LENGTH));
    }
    template <bool writeType>
    uint16_t write(std::ostream& _STREAM) const {
        uint16_t _tellp_beg;
        if constexpr (writeType) {
            _tellp_beg = _STREAM.tellp();
            _STREAM.seekp(2); // 2 bytes for size
            _STREAM.write((char*)&type, 1);
        }
        _length_name = name.length();
		_length_friends = friends.size();
		_STREAM.write((char*)this, 4);
        _STREAM.write(name.c_str(), _length_name);
        for (const auto& f : friends) {
            uint8_t len = f.length();
            _STREAM.write((char*)&len, sizeof(len));
            _STREAM.write(f.c_str(), len);
        }
        pos.write<false>(_STREAM);
        if constexpr (writeType) {
            uint16_t size = _STREAM.tellp().operator-(2 + _tellp_beg); // remove 2 bytes storing the size itself
            _STREAM.seekp(_tellp_beg);
            _STREAM.write((char*)&size, 2);
            return size + 2; // total length includes 2 bytes for size
        }
        return -1;
    }
};

class Players {
public:
    constexpr static uint8_t type = 3;
    	uint8_t mutable _length_players;
    std::vector<Player> players;
    Players()  = default;
    Players(std::istream& _STREAM, const int _STREAM_LENGTH) {
        if (_STREAM.tellg().operator+(1) > _STREAM_LENGTH) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(1)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
		}
		_STREAM.read((char*)this, 1);
        players.reserve(_length_players);
        for (uint8_t i = 0; i < _length_players; ++i)
            players.emplace_back(std::move(Player(_STREAM, _STREAM_LENGTH)));
    }
    template <bool writeType>
    uint16_t write(std::ostream& _STREAM) const {
        uint16_t _tellp_beg;
        if constexpr (writeType) {
            _tellp_beg = _STREAM.tellp();
            _STREAM.seekp(2); // 2 bytes for size
            _STREAM.write((char*)&type, 1);
        }
		_length_players = players.size();
		_STREAM.write((char*)this, 1);
        for (const auto& f : players)
            f.write<false>(_STREAM);
        if constexpr (writeType) {
            uint16_t size = _STREAM.tellp().operator-(2 + _tellp_beg); // remove 2 bytes storing the size itself
            _STREAM.seekp(_tellp_beg);
            _STREAM.write((char*)&size, 2);
            return size + 2; // total length includes 2 bytes for size
        }
        return -1;
    }
};

template <class Sender>
class Dispatcher {
public:
    std::vector<std::function<void(Sender& sender, const PlayerPosition&)>> onPlayerPosition;
	std::vector<std::function<void(Sender& sender, const Player&)>> onPlayer;
	std::vector<std::function<void(Sender& sender, const Players&)>> onPlayers;
	std::vector<std::function<void(Sender& sender, const LoginC2S&)>> onLoginC2S;
    void dispatch(std::istream& _STREAM, uint16_t len, Sender& sender) {
        if (len == 0)
            throw std::exception("Invalid message, received length 0");
        uint8_t type;
        _STREAM.read((char*)&type, 1);
        switch (type) {
            case PlayerPosition::type:
				dispatch(_STREAM, len, sender, onPlayerPosition);
				return;
			case Player::type:
				dispatch(_STREAM, len, sender, onPlayer);
				return;
			case Players::type:
				dispatch(_STREAM, len, sender, onPlayers);
				return;
			case LoginC2S::type:
				dispatch(_STREAM, len, sender, onLoginC2S);
				return;
        }
    }
    template <typename T>
    void dispatch(std::istream& _STREAM, uint16_t len, Sender& sender, const std::vector<std::function<void(Sender& sender, const T&)>>& callbacks) {
        T t(_STREAM, len);
        for (const auto& f : callbacks)
            f(sender, t);
    }
};
};
