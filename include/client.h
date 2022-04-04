#ifndef __FLAPPYBIRD_CLIENT_H__
#define __FLAPPYBIRD_CLIENT_H__

#include <exception>
#include <string>

class Client;

class initialization_error : std::exception {
	friend class Client;

	virtual const char *what() const noexcept override {
		return data.c_str();
	}

private:
	initialization_error(std::string str) : data(std::move(str)) {}

	const std::string data;
};

class Client {
public:
	Client();

	static void initialize();
private:
};

#endif
