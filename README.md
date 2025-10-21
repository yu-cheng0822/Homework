# 41343109 吳育丞
#include<iostream>
using namespace std;
class Polynomial {
private:
	Term* termArray;
	int capacity;
	int terms;
public:
	Polynomial() :capacity(2), terms(0) {
		termArray = new Term[capacity];
	}
	Polynomial Add(Polynomial poly) {
		for (int i = 0; i < capacity; i++) {
			for (int j = 0; j < poly.capacity; j++) {
				if (termArray[i].exp == poly.termArray[j].exp)termArray[i].coef += poly.termArray[j].coef;
			}
		}

		
	}
	Polynomial mult(Polynomial poly);
	float  Eval(float f);
	void newTerm(const float newcoef, int newexp) {
		if (newcoef == 0)return;
		if (terms == capacity) {
			capacity *= 2;
			Term* temp = new Term[capacity];
			copy(termArray, termArray + terms, temp);
			delete[]termArray;
			termArray = temp;
		}
	}
	friend istream& operator>>(iostream& in, Polynomial& poly);
	friend ostream& operator<<(ostream& out, const Polynomial& poly);
};
class Term {
	friend Polynomial;
private:
	float coef;
	int exp;
};
istream& operator>>(istream& in, Polynomial& poly) {
	float coef;
	int exp;
	int n;
	cin >> n;
	while (n--) {
		in >> coef >> exp;
		poly.newTerm(coef, exp);
	}
	return in;
}
ostream& operator<<(ostream& out, const Polynomial& poly) {
	for (int i = 0; i < poly.terms; i++) {
		if (poly.termArray[i].coef < 0 && i>0)out << "-";
		else if (poly.termArray[i].coef > 0 && i>0)out << "+";
		out << poly.termArray[i].coef;
		if (poly.termArray[i].exp != 0)out << "X^" << poly.termArray[i].exp;
	}
	return out;
}
