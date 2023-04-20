#ifndef __STATUS_H
#define __STATUS_H

#include "Header.h"

struct Text
{
	std::string content;
	std::string date_of_release;
};

class Status // text status
{
protected:
	Text text_status;
	Status(const Status& other) = default;
public:
	static const int TYPE_LEN = ELEVEN;
	Status(const std::string& content);
	Status(std::istream& in);
	virtual void printStatus() const;
	void saveDataToFile(std::ostream& file) const;
	bool operator==(const Status& status) const;
	bool operator!=(const Status& status) const { return !(*this == status); }
	virtual void saveExtra(std::ostream& file) const {};
	friend class User;
	friend class Page;
};

class StatusImage : public Status
{
private:
	std::string file_name; //file of image
public:
	StatusImage(const std::string& content, const std::string& file_name);
	StatusImage(std::istream& in);
	StatusImage(const StatusImage& other) : Status(other) { this->file_name = other.file_name; }
	void printStatus() const override;
	void saveExtra(std::ostream& file) const override; // save to the file statusImage data
};

class StatusVideo : public Status
{
private:
	std::string file_name; //file of video
public:
	StatusVideo(const std::string& content, const std::string& file_name);
	StatusVideo(std::istream& in);
	StatusVideo(const StatusVideo& other) : Status(other) { this->file_name = other.file_name; }
	void printStatus() const override;
	void saveExtra(std::ostream& file) const override; // save to the file statusVideo data
};

#endif