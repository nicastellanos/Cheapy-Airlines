/*Prepare software code what implements the Cheapy air booking system.
 Input parameters: x,y,d,z,p_1,...,p_z,C, customer
 login times, P_i max prices to be read from the input file.*/


//Cheapy Airlines Booking System
// Developed By Nicolas Castellanos



#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>


using namespace std;

ofstream fout("output.txt");

/*******************************************************************
 **                       SECTION CLASS                           **
 ******************************************************************/

class section{
private:
    friend class _system;
    int open_seats; //number ;of avaliable seats in the section
    int closed_seats; //number of filled seats in the section
    int current_section; // current section we are in
    int price; // current price of section
public:
    section(int os = 0 , int cs = 0 ,int cur = 0 , int p = 0);
    section& operator=(const section &current);
    bool check_full(); //checks if section is full
    void add_customer(); // adds a customer to the section
    void write_section(ostream &out) const ; //prints list of customers in section
    int return_price(); // returns the price of the cheapest empty section
};


section::section(int os, int cs, int cur, int p){
    open_seats = os;
    closed_seats = cs;
    current_section = cur;
    price = p;
}

section& section::operator=(const section &rhs){
    if(this!= &rhs){
        open_seats = rhs.open_seats;
        closed_seats = rhs.closed_seats;
        current_section = rhs.current_section;
        price = rhs.price;
    }
return *this;
}

bool section::check_full(){ //Checks the status of the current section
    if(open_seats == 0){
        return true; // its full
    }else{
        return false; // its not full
    }
}

int section::return_price(){
    return price;
}

void section::add_customer(void){
    //adds a customer to the list by reducing the number of open seats
    //and incrementing number of closed seats
    open_seats--; //reduced the number of open seats
    closed_seats++; //increments the number of closed seats
}



/*******************************************************************
 **                     FLIGHT CLASS                             **
 ******************************************************************/


class flights { //_system is friend of flights
private:
    friend class _system;
    int max_sec_len;
    int flight_date; //numeric date of flight
    int section_number; // current section
    section* sect_list; // list of the sections of the plane
public:
    bool check_full();
    void write(ostream &out) const ;
    void set(int date,int cs, int max_section_length, int max_seats, int* price_array);
};
////////////// METHOD IMPLEMENTATION OF LOWEST PRICE ////////////
void flights::set(int date, int cs, int max_section_length, int max_seats, int* price_array){
    // this function takes in the date, the current section we are in
    // the max section length and max seats
    // assigns the date for each flight 1-d
    // assigns the number of seats in each section
    //   int p;
    flight_date = date;
    section_number = cs;
    max_sec_len = max_section_length;
    sect_list = new section[max_sec_len];
    for(int i = 0; i<max_sec_len; i++){
        section s(max_seats,0,section_number,price_array[i]);
        sect_list[i] = s;
    }
}


bool flights::check_full(void){
    bool isfull = false;
    for(int i = 0; i< max_sec_len; i++){
        if( sect_list[i].check_full()){ // checking if open seats in the section
            isfull = true; // if no open seats isfull becomes true
        }else{
         isfull = false;
         break;
        }
    }
    
    return isfull; // if no open seats for any section isfull returns true
}

/*******************************************************************
 **                     CUSTOMER CLASS                            **
 ******************************************************************/


class customer{
private:
    friend class _system;
    friend class customer_queue;
    int final_price;
    int max_price;
    int login_time;
    int time_on_system;
    bool processed;
public:
    customer( int mp = 0, int lt = 0);
    int get_time();
    int get_price();
    void set_price(int price);
    customer& operator=( const customer &rhs);
    
};
///////// METHOD IMPLEMENTATION OF CUSTOMER /////////



customer::customer(int mp,int lt){
    //constructor for customer
    max_price = mp;
    login_time = lt;
    time_on_system = 0;
    final_price = 0;
    processed = false;
    
}

customer& customer::operator=(const customer &rhs)
//assignment operator
{
    if(this!= &rhs){
        max_price = rhs.max_price;
        login_time = rhs.login_time;
        time_on_system = rhs.time_on_system;
        
    }
    
    return *this;
}


int customer::get_time(){
    //returns login time of customer
    return login_time;
}

int customer::get_price(){
    //returns max price of custoemr
    return max_price;
}

void customer::set_price(int price){
    //sets the final price of customers ticket
    final_price = price ;
    
}

/*****************************************************************
 **                     CUSTOM_Q CLASS                          **
 ******************************************************************/


class customer_queue {
private:
    friend class _system;
    customer* login_list;
    int length;
    int current_place;
    
public:
    customer_queue( int l = 0, int cp = 0);
    void sort();
    bool is_sorted();
    void read(istream &in, int lo);
    customer pop();
    void write(ostream &out);
    bool isempty();
};
///// METHOD IMPLEMENTATION OF LOGIN_QUEUE ////////////

customer_queue::customer_queue( int l, int cp){
    //constructor
    length = l;
    current_place = cp;
    
}


void customer_queue::sort()
{
    //simple bubble sort algorithm to sort login queue
    int i, j;
    for (i = 0; i < length-1; i++){
        // Last i elements are already in place
        for (j = 0; j < length-i-1; j++){
            if (login_list[j].get_time() > login_list[j+1].get_time())
                swap(login_list[j], login_list[j+1]);
        }
    }
}

void swap(customer &a, customer &b)
{
    // part of bubble sort
    customer temp = a;
    a = b;
    b = temp;
}

customer customer_queue::pop(){
    //pops the next customer from queue
    customer c;
    c = login_list[current_place];
    login_list[current_place].processed = true;
    current_place++;
    return c ;
}


void customer_queue::read(istream &in, int lo) //reads from an infile
//takes the first and sets it to length
//fills vector by pushing the values to the back
//pops the copy and reduced length (idk why but this is the only way it works )
{
    int m,l;//maxprice, login_time, length_on_system
    int i = 0;
    in>>length;
    login_list = new customer[length];
    while(1){
        if(in.eof()) break;
        in>>l>>m;
        customer x(m,l);
        login_list[i] = x;
        i++;
    }
    
}


void customer_queue::write(ostream &out) {
    fout<<"Number of Customers: "<<length<<endl;
    for(int i = 0; i<length; i++){
        out<<"Customer #"<<i+1<<" {";
        out<<login_list[i].get_time()<<","<<login_list[i].get_price()<<"}";
        out<<endl;
        
    }
}

bool customer_queue::isempty(void){
    if(current_place > length){
        return true;
    }else{
        return false;
    }
}

/*******************************************************************
 **                     SYSTEM CLASS                             **
 ******************************************************************/

class _system {
private:
    friend class flights;
    flights* inbound;
    flights* outbound;
    int max_flights;
    int current_ib;
    int current_ob;
    int numb_of_sect;
    int numb_of_seats;
    int system_max_time;
    int wait_time;
    int* price_arr;
public:
    _system();
    void set_up();
    bool find_seat(const customer &a);
    bool check_full_ib();
    bool check_full_ob();
    void purchase();
    void read(istream &in, customer_queue &c);
    void write(ostream &out);
    int get_system_max_time(void);
    int get_wait_time();
};

_system::_system(){
    //constructor
    
}
void _system::set_up(void){ //fills inbound and outbound flights with date information and initializes sections
    int i;
    inbound = new flights[max_flights]; //allocating memory for x flight objects
    outbound = new flights[max_flights];//
    for(i=0;i< max_flights;i++){
        inbound[i].set(i, 0, numb_of_sect, numb_of_seats, price_arr); //
        outbound[i].set(i,0, numb_of_sect, numb_of_seats, price_arr);
    }
}

bool _system::check_full_ib(){ //checks if current inbound plane is full
    bool is_full = false;
    for(int i = 1; i<max_flights;i++){
    if(inbound[i].check_full()){ // if every section in every inbound plane is full
        is_full =  true;
    }else{
        is_full = false;
        break;
        }
    }
    return is_full;
}
bool _system::check_full_ob(){// checks if current outbound plane is full
    bool is_full = false ;
    for(int i = 0; i<= max_flights-1 ;i++){
    // running through the outbound flights
        if(outbound[i].check_full()){ //checking if each plane is full
                                      // by checking if the sections are full
            is_full =  true;
        }else{
        is_full = false;
        break;
        }
    }
    return is_full;
}

int _system::get_system_max_time(void){
    //returns system timeout time
    return system_max_time;
}

bool _system::find_seat(const customer &a){
    bool found_seat = false;
    int price = 0 ; // initalizing price of ticket
    int cur_flight; // variable for the current flight we are on
    int cur_section; // variable for the current section we are on
    for( cur_section = 0; cur_section < numb_of_sect ;cur_section ++){
        // iterating through the sections of the plane starting with the lowest price section
        for(cur_flight = 0; cur_flight <  max_flights; cur_flight ++){ // sweeping through planes from first one to second
                                                                        //last
            
            if(outbound[cur_flight].sect_list[cur_section].return_price() <= a.max_price){ // if the seat price is less the
                                                                                           //the customers price
                if(!(outbound[cur_flight].sect_list[cur_section].check_full())){ // if the current section isnt full
                    found_seat = true;
                    current_ob = cur_flight; // current outbound flight
                    outbound[current_ob].section_number = cur_section; // current section of outbound flight
                    price = outbound[cur_flight].sect_list[cur_section].return_price();
                    
                    break; //breaking the first for loop
                }
            }
        }
        if(found_seat) break;
    }
    
    if(found_seat){ // checking if found seat is false to determine whether to even bother running through inbound list
        found_seat = false; // resets the found seat flag
        for( cur_section = 0; cur_section < numb_of_sect ;cur_section ++){ // current section
            for(cur_flight = current_ob+1 ; cur_flight < max_flights; cur_flight ++){ // sweeping through planes
                if(inbound[cur_flight].sect_list[cur_section].return_price() <= a.max_price ){ // if we find
                                                                                                        //price for
                    if(!(inbound[cur_flight].sect_list[cur_section].check_full())){
                        found_seat = true;
                        current_ib = cur_flight;
                        inbound[current_ib].section_number = cur_section;
                        break;
                    }
                } //what happens when we cant find a flight with the price and then run into the next section
            }
            if (found_seat) break;
        }
    }
    
    fout<<price<<",";
    fout<<a.max_price<<endl;
    return found_seat;
}

void _system::purchase(void){
    int ib_section_number = inbound[current_ib].section_number;
    int ob_section_number = outbound[current_ob].section_number;
    inbound[current_ib].sect_list[ib_section_number].add_customer();
    outbound[current_ob].sect_list[ob_section_number].add_customer();
}

void _system::write(ostream &out) {
    int i,j;
    
    for(i = 0; i<max_flights;i++){
        out<<"Inbound Flight: ";
        out<<inbound[i].flight_date+1<<endl;
        for(j = 0; j<numb_of_sect;j++){
            out<<"Section: "<<j+1<<" ";
            out<<"Price of Section: ";
            out<<inbound[i].sect_list[j].return_price();
            out<<" ";
            out<<"Seats Available: ";
            out<<inbound[i].sect_list[j].open_seats<<endl;
        }
        cout<<endl;
    }
    
    for(i = 0; i<max_flights;i++){
        out<<"Outbound Flight: ";
        out<<outbound[i].flight_date+1<<endl;
        for(j = 0; j<numb_of_sect;j++){
            out<<"Section: "<<j+1<<" ";
            out<<"Price of Section: ";
            out<<outbound[i].sect_list[j].return_price();
            out<<" ";
            out<<"Seats Available: ";
            out<<outbound[i].sect_list[j].open_seats<<endl;
        }
        cout<<endl;
    }
    
}

void _system::read(istream &in, customer_queue &c){
    in>>system_max_time>>wait_time>>max_flights;
    in>>numb_of_sect>>numb_of_seats;
    price_arr = new int[numb_of_sect];
    for(int i = 0; i< numb_of_sect; i++){
        in>>price_arr[i];
    }
    cout<<system_max_time<<","<<max_flights<<","<<numb_of_sect<<","<<numb_of_seats<<endl;
    c.read(in, wait_time);
}

int _system::get_wait_time(){
    
    return wait_time;
    
}



/******************************************************************
 **                                                              **
 ******************************************************************
 
                            MAIN FUNCTION
 
 *******************************************************************
 **                                                               **
 ******************************************************************/


int main(void){
    _system s;
    int time = 0;
    ifstream fin("input.txt");
    customer_queue q; // constructor for q object
    s.read(fin,q); //reads from input file and manipulates
    s.set_up(); //initializes and fills system with data
    customer x; //customer object to manipulate
    q.sort(); // sorting the queue of login customers
    q.write(cout); //writing the list of customers login times and max price
    x = q.pop(); // popping the next customer from the list
    s.write(cout);
    while(1){
        if(s.get_system_max_time() == time){// checking if the system has timed out
            cout<<"System timeout"<<endl;
            break;
        }else{
            if((!s.check_full_ib()) && (!s.check_full_ob())){
            if(x.get_time()+s.get_wait_time() <= time){ // checking if customer login time is up
                if(s.find_seat(x)){
                    cout<<" Seat found! "<<endl; // if seat found under cust criteria
                    s.purchase(); // purchase seat
                    x = q.pop();
                }else{
                    cout<<" No Seat Available\n";
                    x = q.pop();
                }
            }else{
                //do nothing customer has not logged in
            }
        }
    }
    time++;
}
    fout<<"================ FLIGHT INFO ================"<<endl;
    s.write(fout);
    return 0;
}

