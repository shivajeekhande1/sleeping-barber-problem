#include <iostream>
#include <thread>
#include <list>

using namespace std;

//list for waiting hall
std::list<int> waitinghall{};
//customerid which would be incremented
int customerid = 1;
//capacity of waiting hall
int capacity;
//current customerid of a whom the barber is working on
int serving;
//status of barber
string barberstatus = "sleeping";

//function for assigning customer to babber
void serve() {
	//we first see if waiting hall is not empty
	if (waitinghall.size() > 0) {
		//we get the first customer in waiting hall 
		serving = waitinghall.front();
		//change status of barber to serving
		barberstatus = "serving";
		//we remove the customer who barber is serving from waiting hall
		waitinghall.pop_front();
	}
}

//function where each customer is picked for service or waking up and sleeping the barber
void barber() {
	if (waitinghall.size() > 0) {
		//calling serve function to assign the customer
		serve();
		//picking the service randomly between 1 and 5 [1,5 included]
		int servetime = (rand() % 5) + 1;
		//serving the customer for the serve time
		std::this_thread::sleep_for(std::chrono::seconds(servetime));
	}
	//if waiting hall is empty putting the barber to sleep
	else {
		serving = 0;
		barberstatus = "sleeping";
	}
}

//function for putting the customers in the waiting hall
void waiting() {
	//checking if waiting hall is full
	if (waitinghall.size() < capacity) {
		//placing the customer at the end of the line in waiting hall
		waitinghall.push_back(customerid);
		//incrementing customer id for the n ext customer
		customerid += 1;
	}
	else {
		//customers leaving the shop because of filled waiting hall
		cout << customerid << "left due to lack of place\n";
		//incrementing the customer id for next customer
		customerid += 1;
	}
}

//function for sending a new customer every 3 seconds
void customerarrival() {
	//calling waiting function for arranging in waiting hall
	waiting();
	//putting the function to sleep for 3 seconds and then calling for the next customer
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

//function for printing the status of the shop every second
void print() {
	if (barberstatus != "sleeping") {
		//printing the current customer who is being served
		cout << "barber is serving" << serving << "\n";
		//printing the current status of waiting hall
		cout << "waiting hall: ";
		for (auto v : waitinghall)
			std::cout << v << " ";
		cout << "\n\n\n";
	}
	else {
		//printing if barber is sleeping
		cout << "barber is sleeping\n";
		cout << "waiting hall:\n\n\n";
	}
	//putting the thread to sleep for one second so that it is called after every second
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
	//asking for the input of the capacity of the waiting hall
	cout << "enter capacity of waiting hall: ";
	cin >> capacity;
	
	//running the barber function indefinatly so that he is either sleeping or working
	std::thread barberobj([] {while (true)barber(); });
	//running the customer arrival thread indefinatly as custmer keeps arriving every three seconds
	std::thread customerarrivalobj([]{while (true)customerarrival();});
	//running the printing thread every second so the status is printed every second
	std::thread printobj([] {while (true)print(); });
	
	//joining all the three threads
	barberobj.join();
	customerarrivalobj.join();
	printobj.join();
	
	system("pause");
	return 0;
}