#include <iostream>
#include <string>
#include "lib/request/request.h"
#include "../../Queue/src/lib/queue/queue.h"

#define REQ_FILENAME "C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Queue_Reqs/data/requests.csv"

using std::cout, std::string;

int main() {
    string text = "Principal,title_here,desc_here,content_here";
    Request my_req(text, ',');
    cout << sender_to_string(my_req.sender) << '\n';
    cout << my_req.content << '\n';
    Queue<Request> my_queue = QueueFn::from_reqs_file(REQ_FILENAME, ',');

}