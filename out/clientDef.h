

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

namespace NS_Client {

class LoginConfirmS2C {
public:
    constexpr static uint8_t type = 5;
    LoginConfirmS2C()  = default;
    LoginConfirmS2C(std::istream& _STREAM, const int _STREAM_LENGTH) {
        uint16_t _tellg_beg = _STREAM.tellg();
        if (_STREAM.tellg().operator+(0) > _STREAM_LENGTH + _tellg_beg) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(0)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
		}
		_STREAM.read((char*)this, 0);
    }
};

class LoginC2S {
public:
    constexpr static uint8_t type = 4;
    	uint8_t mutable _length_username;
	uint8_t mutable _length_password;
    std::string username;
	std::string password;
    LoginC2S() = default;
    LoginC2S(std::string&&  username,std::string&&  password)  : username(std::move(username)),password(std::move(password)),_length_username(username.length()),_length_password(password.length()) {}
    template <bool writeType>
    uint16_t write(std::ostream& _STREAM) const {
        uint16_t _tellp_beg;
        if constexpr (writeType) {
            _tellp_beg = _STREAM.tellp();
            _STREAM.seekp(2); // 2 bytes for size
            _STREAM.write((char*)&type, 1);
        }
        _length_username = username.length();
_length_password = password.length();
		_STREAM.write((char*)this, 2);
        _STREAM.write(username.c_str(), _length_username);_STREAM.write(password.c_str(), _length_password);
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
        uint16_t _tellg_beg = _STREAM.tellg();
        if (_STREAM.tellg().operator+(5) > _STREAM_LENGTH + _tellg_beg) {
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
        uint16_t _tellg_beg = _STREAM.tellg();
        if (_STREAM.tellg().operator+(4) > _STREAM_LENGTH + _tellg_beg) {
			std::string ERRMSG = std::to_string(_STREAM.tellg()) + "," + std::to_string(4)+ "," + std::to_string(_STREAM_LENGTH);
			throw std::out_of_range(ERRMSG);
		}
		_STREAM.read((char*)this, 4);
        {
			uint8_t L = _length_name;
			if (_STREAM.tellg().operator+(L) > _STREAM_LENGTH + _tellg_beg) {
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
            if (_STREAM.tellg().operator+(1) > _STREAM_LENGTH + _tellg_beg) {
                std::string ERRMSG = std::to_string(_STREAM.tellg()) + ",1," + std::to_string(_STREAM_LENGTH);
                throw std::out_of_range(ERRMSG);
            }
            _STREAM.read((char*)&LEN, 1);
            if (_STREAM.tellg().operator+(LEN) > _STREAM_LENGTH + _tellg_beg) {
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
        uint16_t _tellg_beg = _STREAM.tellg();
        if (_STREAM.tellg().operator+(1) > _STREAM_LENGTH + _tellg_beg) {
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

class Dispatcher {
public:
    std::vector<std::function<void(const PlayerPosition&)>> onPlayerPosition;
	std::vector<std::function<void(const Player&)>> onPlayer;
	std::vector<std::function<void(const Players&)>> onPlayers;
	std::vector<std::function<void(const LoginConfirmS2C&)>> onLoginConfirmS2C;
    void dispatch(std::istream& _STREAM, uint16_t len) {
        if (len == 0)
            throw std::exception("Invalid message, received length 0");
        uint8_t type;
        _STREAM.read((char*)&type, 1);
        switch (type) {
            case PlayerPosition::type:
				dispatch(_STREAM, len - 1, onPlayerPosition);
				return;
			case Player::type:
				dispatch(_STREAM, len - 1, onPlayer);
				return;
			case Players::type:
				dispatch(_STREAM, len - 1, onPlayers);
				return;
			case LoginConfirmS2C::type:
				dispatch(_STREAM, len - 1, onLoginConfirmS2C);
				return;
        }
    }
    template <typename T>
    void dispatch(std::istream& _STREAM, uint16_t len, const std::vector<std::function<void(const T&)>>& callbacks) {
        const T t(_STREAM, len);
        for (const auto& f : callbacks)
            f(t);
    }
};
};
