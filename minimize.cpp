#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

struct cube {
	vector<int> minterms;
	vector<int> difference;
	bool checked_off;
}; 

vector<cube> zero, one, two, three, four, five, six, seven, eight, nine, ten, not_used, orig, temp;
bool printed;

void minimize(string input);
void process_minterms(string input);
int count_ones(int number);
void reduce_cubes(int count);
void compare_cubes(vector<cube>& v1, vector<cube>& v2);
void add_unused(vector<cube>& v);
void vector_sort(vector<cube>& v, int p, int q);
int vector_partition(vector<cube>& v, int p, int q);
void sort_vectors();
void minterm_sort(vector<int>& v, int p, int q);
int minterm_partition(vector<int>& v, int p, int q);
string find_minterm_value(vector<int>& v, int count);
void assess_minterms(int count);
void print_value(cube term, int count);

int main(int argc, char *argv[])
{
	printed = false;

	if(argc == 1){
		cout << "Please type in a function to be minimized." << endl;
		string input = "";
		getline(cin, input);
		minimize(input);
	} else if (argc == 2) {
		ifstream file(argv[2]);
		string input = "";
		if(!file){
			cout << "Please run again with a file name ";
			cout << "or wait for prompt to enter input." << endl;
		}
		while(getline(file, input)){
			minimize(input);
		}
		file.close();
	} else {
		cout << "Please only input 1 file at a time. Thanks!" << endl;
	}

	cout << endl;
}

void minimize(string input)
{
	process_minterms(input);
	sort_vectors();
	
	int count = 11;

	if(ten.empty()){
		count--;
		if(nine.empty()){
			count--;
			if(eight.empty()){
				count--;
				if(seven.empty()){
					count--;
					if(six.empty()){
						count--;
						if(five.empty()){
							count--;
							if(four.empty()){
								count--;
								if(three.empty()){
									count--;
									if(two.empty()){
										count--;
										if(one.empty()){
											count--;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for(int i = count; i > 0; i--){
		reduce_cubes(i);
	}

	add_unused(ten);	
	assess_minterms(count);
}

void process_minterms(string input)
{
	bool dont_care = false;
	
	for(unsigned int i = 0; i < input.length(); i++){
		char c = input[i];
		if(c != 109 && c != 40 && c != 41 && c != 43 && c != 44){
			if(c == 100){
				dont_care = true;
			} else {
				int character = c - '0';
				cube term;
				term.minterms.push_back(character);
				term.difference.push_back(0);
				if(! dont_care){
					term.checked_off = false;
				} else {
					term.checked_off = true;
				}
				int num_ones = count_ones(character);
				if(num_ones == 0){
					zero.push_back(term);
				} else if(num_ones == 1){
					one.push_back(term);
				} else if(num_ones == 2){
					two.push_back(term);
				} else if(num_ones == 3){
					three.push_back(term);
				} else if(num_ones == 4){
					four.push_back(term);
				} else if(num_ones == 5){
					five.push_back(term);
				} else if(num_ones == 6){
					six.push_back(term);
				} else if(num_ones == 7){
					seven.push_back(term);
				} else if(num_ones == 8){
					eight.push_back(term);
				} else if(num_ones == 9){
					nine.push_back(term);
				} else {
					ten.push_back(term);
				}
				orig.push_back(term);
			}
		}
	}
}

int count_ones(int number)
{
	int bit = 0;
	int count = 0;

	while(number > 0){
		bit = number % 2;
		number = number >> 1;
		if(bit){
			count++;
		}
	}
	
	return count;
}

void reduce_cubes(int count)
{
	compare_cubes(zero, one);

	if(count >= 2)
		compare_cubes(one, two);
	if(count >= 3)
		compare_cubes(two, three);
	if(count >= 4)
		compare_cubes(three, four);
	if(count >= 5)
		compare_cubes(four, five);
	if(count >= 6)
		compare_cubes(five, six);
	if(count >= 7)
		compare_cubes(six, seven);
	if(count >= 8)
		compare_cubes(seven, eight);
	if(count >= 9)
		compare_cubes(eight, nine);
	if(count >= 10)
		compare_cubes(nine, ten);
}

void compare_cubes(vector<cube>& v1, vector<cube>& v2)
{
	for(unsigned int i = 0; i < v1.size(); i++){
		for(unsigned int j = 0; j < v2.size(); j++){
			bool valid = true;
			if(v1[i].difference == v2[j].difference && v1[i].difference.size() != 0){
				assert(v1[i].minterms.size() == v2[j].minterms.size());	
				int diff = v2[j].minterms[0] - v1[i].minterms[0];
				for(unsigned int k = 0; k < v1[i].minterms.size(); k++){
					if(v2[j].minterms[k] - v1[i].minterms[k] != diff)
						valid = false;
				}
				if(valid){
					v1[i].checked_off = true;
					v2[j].checked_off = true;
					v1[i].difference.push_back(diff);
					v1[i].minterms.insert(v1[i].minterms.end(), v2[j].minterms.begin(), v2[j].minterms.end());
					minterm_sort(v1[i].minterms, 0, v1[i].minterms.size());
					temp.push_back(v1[i]);
				}
			}
		}
	}
	add_unused(v1);
	v1.clear();
	v1 = temp;
	temp.clear();

}

void add_unused(vector<cube>& v)
{
	for(unsigned int i = 0; i < v.size(); i++){
		if(!v[i].checked_off){
			not_used.push_back(v[i]);
		}
	}
}

void vector_sort(vector<cube>& v, int p, int q)
{
	if(p < q){
		int r = vector_partition(v, p, q);
		vector_sort(v, p, r);
		vector_sort(v, r + 1, q);
	}
}

int vector_partition(vector<cube>& v, int p, int q)
{
	int i = v[p].minterms[0];
	int j = p;
	
	for(int k = p + 1; k < q; k++){
		if(v[p].minterms[0] <= i){
			j++;
			swap(v[j], v[k]);
		}
	}

	swap(v[j], v[p]);

	return j;
}

void sort_vectors()
{
	vector_sort(zero, 0, zero.size());
	vector_sort(one, 0, one.size());
	vector_sort(two, 0, two.size());
	vector_sort(three, 0, three.size());
	vector_sort(four, 0, four.size());
	vector_sort(five, 0, five.size());
	vector_sort(six, 0, six.size());
	vector_sort(seven, 0, seven.size());
	vector_sort(eight, 0, eight.size());
	vector_sort(nine, 0, nine.size());
	vector_sort(ten, 0, ten.size());
}

void minterm_sort(vector<int>& v, int p, int q)
{
	if(p < q){
		int r = minterm_partition(v, p, q);
		minterm_sort(v, p, r);
		minterm_sort(v, r + 1, q);
	}
}

int minterm_partition(vector<int>& v, int p, int q)
{
	int i = v[p];
	int j = p;

	for(int k = p + 1; k < q; k++){
		if(v[p] <= i){
			j++;
			swap(v[j], v[k]);
		}
	}

	swap(v[j], v[p]);

	return j;
}

string find_minterm_value(vector<int>& v, int count)
{
	string value = "";

	for(int i = count - 1; i >= 0; i--){
		unsigned short mask = 0x1;
		mask = mask << i;
		int init = (unsigned short)v[0] & mask;
		bool valid = true;
		for(unsigned int j = 1; j < v.size(); j++){
			if((v[j] & mask) != init){
				valid = false;
			}
		}
		if(valid){
			char val = '0' + init;
			value.push_back(val);
		}else 
			value += "-";
	}

	return value;
}

void assess_minterms(int count)
{
	for(unsigned int i = 0; i < orig.size(); i++){
		int term = orig[i].minterms[0];
		orig[i].difference.clear();
		for(unsigned int j = 0; j < not_used.size(); j++){
			bool added = false;
			for(unsigned int k = 0; k < not_used[j].minterms.size(); k++){
				if(!added && not_used[j].minterms[k] == term)
					orig[i].difference.push_back(j);
			}
		}
	}
		
	for(unsigned int i = 0; i < orig.size(); i++){
		if(orig[i].difference.size() == 1){
			vector<int> term = not_used[orig[i].difference[0]].minterms;
			print_value(not_used[orig[i].difference[0]], count);
			orig[i].checked_off = true;
			not_used[orig[i].difference[0]].checked_off = true;
			for(unsigned int j = 0; j < orig.size(); j++){
				for(unsigned int k = 0; k < orig[j].difference.size(); k++){
					if(not_used[orig[j].difference[k]].minterms == term){
						orig[j].checked_off = true;
					}
				}
			}
		}
	}

	for(unsigned int i = 0; i < orig.size(); i++){
		if(orig[i].checked_off == false){
			vector<int> term = not_used[orig[i].difference[0]].minterms;
			print_value(not_used[orig[i].difference[0]], count);
			orig[i].checked_off = true;
			not_used[orig[i].difference[0]].checked_off = true;
			for(unsigned int j = 0; j < orig.size(); j++){
				for(unsigned int k = 0; k < orig[j].difference.size(); k++){
					if(not_used[orig[j].difference[k]].minterms == term){
						orig[j].checked_off = true;
					}
				}
			}
		}
	}
											
}

void print_value(cube term, int count)
{
	string value = find_minterm_value(term.minterms, count);
	if(printed)
		cout << "+";

	for(unsigned int i = 0; i < value.length(); i++){
		if(i == 0){
			if(value[0] == '0')
				cout << "A'";
			if(value[0] == '1')
				cout << "A";
		} else if(i == 1){
			if(value[1] == '0')
				cout << "B'";
			if(value[1] == '1')
				cout << "B";
		} else if(i == 2){
			if(value[2] == '0')
				cout << "C'";
			if(value[2] == '1')
				cout << "C";
		} else if(i == 3){
			if(value[3] == '0')
				cout << "D'";
			if(value[3] == '1')
				cout << "D";
		} else if(i == 4){
			if(value[4] == '0')
				cout << "E'";
			if(value[4] == '1')
				cout << "E";
		} else if(i == 5){
			if(value[5] == '0')
				cout << "F'";
			if(value[5] == '1')
				cout << "F";
		} else if(i == 6){
			if(value[6] == '0')
				cout << "G'";
			if(value[6] == '1')
				cout << "G";
		} else if(i == 7){
			if(value[7] == '0')
				cout << "H'";
			if(value[7] == '1')
				cout << "H";
		} else if(i == 8){
			if(value[8] == '0')
				cout << "I'";
			if(value[8] == '1')
				cout << "I";
		} else {
			if(value[9] == '0')
				cout << "J'";
			if(value[9] == '1')
				cout << "J";
		}	
	}

	printed = true;
}

