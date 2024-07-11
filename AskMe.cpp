#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <map>
using namespace std;
class Helper
{
public:
	static vector<string> ReadFileLines(string file_name)
	{
		vector<string> lines;
		string line;
		ifstream file_handler(file_name);
		while (getline(file_handler, line))
			lines.push_back(line);
		return lines;
	}
	static vector<string> SplitString(string line, char delimiter = ',')
	{
		vector<string> tokens;
		istringstream iss(line);
		string token;
		while (getline(iss, token, delimiter))
			tokens.push_back(token);
		return tokens;
	}

	static void WriteFileLines(string file_name, vector<string> lines, bool append = false)
	{

		auto status = ios::in | ios::out | ios::trunc;
		if (append)
			status = ios::in | ios::out | ios::app;

		fstream file_handler(file_name, status);
		for (auto& line : lines)
			file_handler << line << endl;

	}
	static int ReadInt(int low, int high) {
		cout << "\nEnter number in range " << low << " - " << high << ": ";
		int value;

		cin >> value;

		if (low <= value && value <= high)
			return value;

		cout << "ERROR: invalid number...Try again\n";
		return ReadInt(low, high);
	}
	static int ShowMenu(vector<string> choices) {
		cout << "\nMenu:\n";
		for (int choice = 0; choice < (int)choices.size(); ++choice) {
			cout << "\t" << choice + 1 << ": " << choices[choice] << "\n";
		}
		return ReadInt(1, choices.size());
	}

};
class Userinteraction
{
public:
	static pair<string, string> GetUserLoginInfo()
	{
		string username, password;
		cout << "Enter Username: ";
		cin >> username;
		cout << "\nEnter Password: ";
		cin >> password;
		cout << "\n";
		return { username , password };
	}
	static string GetUserSignUpInfo()
	{
		string line;
		string name, email, username, password;
		string allow_anonymous_questions;
		cout << "Enter name: ";
		cin >> name;
		line += name;
		line += ',';
		cout << "\nEnter email:";
		cin >> email;
		line += email;
		line += ',';
		cout << "\n Enter Username: ";
		cin >> username;
		line += username;
		line += ',';
		cout << "\n Enter password: ";
		cin >> password;
		line += password;
		line += ',';
		cout << "\n Do you Allow anonymous questions? (0 or 1)?: ";
		cin >> allow_anonymous_questions;
		line += allow_anonymous_questions;

		return line;
	}
};
class User {
private:
	int DATA_VARIABLES_NUM = 6;
	int id;
	string name, email, username, password;
	bool is_allow_aq;
	vector<int> from_ids;
	map<int, vector<int>> mp_qid_questionNItsThreads_to;
	map<int, vector<int>> mp_fromqid_relatedthreads;
public:
	User() = default;
	User(string line)
	{
		vector<string> data = Helper::SplitString(line);
		if (data.size() == 6)
		{
			id = stoi(data[0]);
			name = data[1];
			email = data[2];
			username = data[3];
			password = data[4];
			is_allow_aq = stoi(data[5]);
		}
		else if (data.size() == 5)
		{
			name = data[0];
			email = data[1];
			username = data[2];
			password = data[3];
			is_allow_aq = stoi(data[4]);
		}
	}
	int GetID() const
	{
		return id;
	}
	vector<int> get_fromids()
	{
		return from_ids;
	}
	void clear_fromids()
	{
		from_ids.clear();
	}
	void add_fromids(int val)
	{
		from_ids.push_back(val);
	}
	map<int, vector<int>>& Get_mp_qid_thread()
	{
		return mp_qid_questionNItsThreads_to;
	}
	map<int, vector<int>>& Get_mp_fromqid_relatedthreads()
	{
		return mp_fromqid_relatedthreads;
	}
	void clear_mp_qid_thread()
	{
		mp_qid_questionNItsThreads_to.clear();
	}
	void add_mp_qid_thread(pair<int, vector<int>> p)
	{
		mp_qid_questionNItsThreads_to.insert(p);
	}
	string GetUsername() const
	{
		return username;
	}
	string GetPassword() const
	{
		return password;
	}
	string GetName() const
	{
		return name;
	}
	void SetId(int id_)
	{
		id = id_;
	}
	string toString()
	{
		ostringstream oss;
		oss << GetID() << "," << GetName() << "," << email << "," << GetUsername() << "," << GetPassword() << "," << is_allow_aq;
		return oss.str();
	}

};
class Question
{
private:
	int DATA_VARIABLES_NUM = 7;
	int id, from_user_id, to_user_id, is_anonymous, parent_id;
	string q_text, q_answer;
public:
	Question() = default;
	Question(string line)
	{
		vector<string> data = Helper::SplitString(line);
		assert(data.size() == DATA_VARIABLES_NUM);
		id = stoi(data[0]);
		from_user_id = stoi(data[1]);
		to_user_id = stoi(data[2]);
		q_text = data[3];
		q_answer = data[4];
		is_anonymous = stoi(data[5]);
		parent_id = stoi(data[6]);
	}
	int GetId() const
	{
		return id;
	}
	int GetFromUserId() const
	{
		return from_user_id;
	}
	int GetToUserId() const
	{
		return to_user_id;
	}
	int GetParentID()
	{
		return parent_id;
	}
	void SetAnswer(string ans)
	{
		q_answer = ans;
	}
	string GetAnswer()
	{
		return q_answer;
	}
	void PrintFromQuestion()
	{
		cout << "Question ID (" << GetId() << ") ";
		if (!is_anonymous)
			cout << " !AQ ";
		cout << "to user id (" << GetToUserId() << ")\t Question: " << q_text;
		if (q_answer == "")
			cout << "\tNOT answered YET";
		else
			cout << "\tAnswer: " << q_answer;
		cout << "\n\n";
	}
	void PrintToQuestion()
	{
		string temp = "";

		if (parent_id != -1)
			temp = "\tThread: ";

		cout << temp << " Question ID (" << GetId() << ") ";
		if (!is_anonymous)
			cout << "from user id (" << GetFromUserId() << ")";;
		cout << "\t Question: " << q_text;
		if (q_answer != "")
			cout << "\n\t" << temp << " Answer: " << q_answer;
		cout << "\n\n";
	}
	void PrintFeedQuestion() {
		if (parent_id != -1)
			cout << "Thread Parent Question ID (" << parent_id << ") ";

		cout << "Question Id (" << GetId() << ")";
		if (!is_anonymous)
			cout << " from user id(" << from_user_id << ")";

		cout << " To user id(" << to_user_id << ")";

		cout << "\t Question: " << q_text << "\n";
		if (q_answer != "")
			cout << "\tAnswer: " << q_answer << "\n";
	}

	string ToString()
	{
		ostringstream oss;
		oss << id << "," << from_user_id << "," << to_user_id << "," << q_text << "," << q_answer << "," << is_anonymous << "," << parent_id;
		return oss.str();
	}
};
class UserManager
{
private:
	bool is_data_loaded;
	map<string, User> mp_username_user;//i need this map for login
	map<int, User> mp_userid_user;
	User current_user;
	int last_id_readed;
public:
	void AccessSystem() {
		int choice = Helper::ShowMenu({ "Login", "Sign Up" });
		if (choice == 1)
			Login();
		else
			SignUp();
	}
	UserManager()
	{
		is_data_loaded = false;
		LoadData();
	}
	void LoadData()
	{
		mp_username_user.clear();
		vector<string> lines = Helper::ReadFileLines("users.txt");
		for (auto& line : lines)
		{
			User u(line);
			mp_username_user[u.GetUsername()] = u;
			mp_userid_user[u.GetID()] = u;
			last_id_readed = u.GetID();
		}
		is_data_loaded = true;
	}
	void Login()
	{
		Sync();
		while (true)
		{
		pair<string, string> p = Userinteraction::GetUserLoginInfo();
			if (!mp_username_user.count(p.first))
			{
				cout << "\nUsername Not Found\n\n";
				continue;
			}
			if (mp_username_user[p.first].GetPassword() == p.second)
			{
				cout << "\nWelcome, " << mp_username_user[p.first].GetName() << "!\n\n";
				current_user = mp_username_user[p.first];
				return;
			}
			cout << "\nPassword is incorrect!\n\n";
		}
	}
	void SignUp()
	{
		Sync();
		string line = Userinteraction::GetUserSignUpInfo();
		User u(line);
		current_user = u;
		current_user.SetId(++last_id_readed);
		mp_username_user[current_user.GetUsername()] = current_user;
		vector<string> lines_to_append;
		lines_to_append.push_back(current_user.toString());
		Helper::WriteFileLines("users.txt", lines_to_append, true);

	}
	void ListSystemUsers()
	{
		for (auto& pair : mp_username_user)
			cout << "ID: " << pair.second.GetID() << "\t Name: " << pair.second.GetName() << endl;
	}
	void Sync()
	{
		if (!is_data_loaded)
			LoadData();
	}
	User& GetCurrentUser()
	{
		return current_user;
	}
	map<int, User>& GetUserIdUserMap()
	{
		return mp_userid_user;
	}
};

class QuestionManager
{
private:
	map<int, Question> mp_questionid_question;
	bool is_data_loaded = false;
	bool is_questionsOfUser_filled = false;
	int last_id_readed;
public:
	QuestionManager()
	{
		LoadData();
	}
	void FillUserQuestions(User& user)
	{
		user.clear_fromids();
		user.clear_mp_qid_thread();
		for (auto& pair : mp_questionid_question)
		{
			if (pair.second.GetFromUserId() == user.GetID()) //fill questions from him
			{
				user.add_fromids(pair.second.GetId());
			}
			if (pair.second.GetToUserId() == user.GetID()) //fill questions to him and all it's threads
			{
				if (pair.second.GetParentID() == -1)
					user.Get_mp_qid_thread()[pair.second.GetId()].push_back(pair.second.GetId());
				else
					user.Get_mp_qid_thread()[pair.second.GetParentID()].push_back(pair.second.GetId());
			}
		}
		for (auto& id : user.get_fromids())
		{
			if (mp_questionid_question[id].GetParentID() == -1) //this is a parent q , now search for it's threads that has parent id of "id"
			{
				user.Get_mp_fromqid_relatedthreads()[id].push_back(mp_questionid_question[id].GetId());
				for (auto& pair : mp_questionid_question)
				{
					if (pair.second.GetParentID() == id)
					{
						user.Get_mp_fromqid_relatedthreads()[id].push_back(pair.second.GetId());
					}
				}
			}
		}
		is_questionsOfUser_filled = true;
	}
	void LoadData()
	{

		mp_questionid_question.clear();
		vector<string> lines = Helper::ReadFileLines("questions.txt");
		for (auto& line : lines)
		{
			Question q(line);
			mp_questionid_question[q.GetId()] = q;
			last_id_readed = q.GetId();
		}
		is_data_loaded = true;
	}
	void PrintFromQuestions(User& user)
	{
		Sync(user);
		if (user.get_fromids().empty())
		{
			cout << "\nNo Questions From You Yet.\n";
			return;
		}
		for (auto& id : user.get_fromids())
		{
			mp_questionid_question[id].PrintFromQuestion();
		}

	}
	void PrintToQuestions(User& user)
	{
		Sync(user);
		if (user.Get_mp_qid_thread().empty())
		{
			cout << "\nNo Questions To You Until now\n";
		}
		for (auto& pair : user.Get_mp_qid_thread())
		{
			for (auto& id : pair.second)
			{
				mp_questionid_question[id].PrintToQuestion();
			}
		}
	}
	void AnswerQuestion(User& user)
	{
		Sync(user);
		int q_id;
		string ans;
		cout << "Enter question id or -1 to cancel: ";
		cin >> q_id;
		if (q_id == -1)
			return;
		Question& q = mp_questionid_question[q_id];
		if (q.GetToUserId() == user.GetID())
		{
			q.PrintToQuestion();
			if (q.GetAnswer() != "")
				cout << "Warning: Already Answered , Answer will be updated\n";
			cout << "Enter Answer: ";
			getline(cin, ans);
			getline(cin, ans);
			q.SetAnswer(ans);
			UpdateQuestionFile();
		}


	}
	void AskQuestion(User& user, UserManager& users_manager)
	{
		Sync(user);
		string line;
		ostringstream oss(line);
		Question question;
		int to_user_id;
		int parent_id;
		int from_user_id = user.GetID();
		int q_id = ++last_id_readed;
		string q_text;
		int aq;
		cout << "Enter user id or -1 to cancel: ";
		cin >> to_user_id; //i need to knows if he exists and if he allows aq or not
		if (to_user_id == -1)
			return;
		if (users_manager.GetUserIdUserMap().count(to_user_id))
		{
			FillUserQuestions(users_manager.GetUserIdUserMap()[to_user_id]);
			cout << "For Thread Question Enter Question Id or -1 for a new question: ";
			cin >> parent_id;
			while (users_manager.GetUserIdUserMap()[to_user_id].Get_mp_qid_thread().find(parent_id) == users_manager.GetUserIdUserMap()[to_user_id].Get_mp_qid_thread().end() && parent_id != -1)
			{
				cout << "Sorry This Parent Id Doesn't belong to this user\n";
				cout << "For Thread Question Enter Question Id or -1 for a new question: ";
				cin >> parent_id; //i need to know if this parent question belongs to the user that will be asked the question (done)
			}
			cout << "Enter Question Text: ";
			getline(cin, q_text);
			getline(cin, q_text);
			cout << "Do you want it anonymous? (0 or 1)?:";
			cin >> aq;
			oss << q_id << "," << from_user_id << "," << to_user_id << "," << q_text << "," << "" << "," << aq << "," << parent_id;
			question = Question(oss.str());
			mp_questionid_question[question.GetId()] = question;
			UpdateQuestionFile();
			FillUserQuestions(user);
		}
		else
		{
			cout << "\nSorry, User Not Found\n";
			return;
		}

	}
	void DeleteQuestion(User& user)
	{
		Sync(user);
		int q_id;
		cout << "Enter question id or -1 to cancel: ";
		cin >> q_id;
		if (q_id == -1)
			return;
		if (mp_questionid_question[q_id].GetFromUserId() == user.GetID())
		{
			auto it = mp_questionid_question.find(q_id); //after i found the question ,if it is thread i will continue the below 
			if (it != mp_questionid_question.end()) // if question is found
			{
				if (it->second.GetParentID() != -1) //thread case
				{ // means it is a thread
					mp_questionid_question.erase(it);
					UpdateQuestionFile();
					FillUserQuestions(user);
					//cout << "\nThread Question Deleted Successfully\n";
				}
				else //parent question case
				{
					map<int, vector<int>>& mp = user.Get_mp_fromqid_relatedthreads(); // i need to make a map all parent ids from a specific user and it's related threads
					for (auto& pair : mp)
					{
						for (auto id : pair.second)//vector of ids , i need to find all of it and erase it
						{
							it = mp_questionid_question.find(id);
							if (it != mp_questionid_question.end()) {
								mp_questionid_question.erase(it);
								
							}
						}
						UpdateQuestionFile();
						FillUserQuestions(user);
					}
				}
			}
		}
		else {
			cout << "\nYou don't have authorization to delete this question!\n";
		}
	}
	void ListUsersFeed()
	{
		for (auto& pair : mp_questionid_question)
		{
			pair.second.PrintFeedQuestion();
		}
	}
	void Sync(User& user)
	{
		if (!is_data_loaded)
			LoadData();
		FillUserQuestions(user);
	}
	void UpdateQuestionFile()
	{
		vector<string> question_lines;
		for (auto& pair : mp_questionid_question)
		{
			question_lines.push_back(pair.second.ToString());
		}
		Helper::WriteFileLines("questions.txt", question_lines);
		is_data_loaded = false;
	}
};

class AskMeSystem {
private:
	UserManager users_manager;
	QuestionManager questions_manager;

public:

	void runSystem() {
		users_manager.AccessSystem();
		vector<string> menu;
		menu.push_back("Print Questions To Me");
		menu.push_back("Print Questions From Me");
		menu.push_back("Answer Question");
		menu.push_back("Delete Question");
		menu.push_back("Ask Question");
		menu.push_back("List System Users");
		menu.push_back("Feed");
		menu.push_back("Logout");

		while (true) {
			int choice = Helper::ShowMenu(menu);
			if (choice == 1)
				questions_manager.PrintToQuestions(users_manager.GetCurrentUser());
			else if (choice == 2)
				questions_manager.PrintFromQuestions(users_manager.GetCurrentUser());
			else if (choice == 3)
				questions_manager.AnswerQuestion(users_manager.GetCurrentUser());
			else if (choice == 4)
				questions_manager.DeleteQuestion(users_manager.GetCurrentUser());
			else if (choice == 5)
				questions_manager.AskQuestion(users_manager.GetCurrentUser(), users_manager); //i need somehow to check if user question is asked to exists
			else if (choice == 6)
				users_manager.ListSystemUsers();
			else if (choice == 7)
				questions_manager.ListUsersFeed();
			else
				break;
		}
		runSystem();
	}
};

int main()
{
	AskMeSystem system;
	system.runSystem();
}

