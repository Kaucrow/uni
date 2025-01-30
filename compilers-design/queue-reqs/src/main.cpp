#include <iostream>
#include <iomanip>
#include <string>
#include "../../../../tools/cpp/termcolor.hpp"
#include "lib/request/request.h"
#include "../../Queue/src/lib/queue/queue.h"
#include "../../Linked_List/src/linked_list/linked_list.h"

#define color termcolor
#define REQ_FILENAME "C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Queue_Reqs/data/requests.csv"

using std::cout, std::string;

int main() {
    Queue<Request> queue = QueueFn::from_reqs_file(REQ_FILENAME, ',');
    queue.sort([](const Request &a, const Request &b) {
        return a.sender >= b.sender;
    });
    for(int i = queue.len(); i > 0; i--) {
        Request req = queue.pop();
        LinkedList<string> content_lines;
        while(req.content.length() >= 75) {
            content_lines.append(req.content.substr(0, 75));
            req.content = req.content.substr(75);
        }
        content_lines.append(req.content);
        cout    << std::setw(85) << std::setfill('-') << "\n" << std::setfill(' ')
                << "| " << color::red << "Sender: " << color::white << std::left << std::setw(72) << sender_to_string(req.sender) << " |\n"
                << "| " << color::cyan << "Title: " << color::white << std::setw(73) << req.title << " |\n"
                << "| " << color::green << "Description: " << color::white << std::setw(67) << req.description << " |\n"
                << "| " << color::yellow << "Content:" << color::white << std::right << std::setw(75) << " |\n" << std::left;

        for(auto &line : content_lines) 
            cout << "| " << std::setw(80) << line << " |\n";

        cout << std::right << std::setw(86) << std::setfill('-') << "\n\n" << std::setfill(' ');
    }
}