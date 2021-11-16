#include<iostream>
#include<math.h>

using namespace std;

class Polynomial {
protected:
    class Term {
    protected:
        int exponent;
        int coefficient;
        Term *next;
        Term(int exp, int coeff, Term *n) : exponent(exp), coefficient(coeff), next(n) {}

        friend class Polynomial;

        friend Polynomial operator + (const Polynomial &p, const Polynomial &q);
        friend Polynomial operator * (const Polynomial &p, const Polynomial &q);
        friend ostream & operator << (ostream &out, const Polynomial &p);
    };

    Term *head;

    void copy(const Polynomial &p) {
        Term *q = p.head;
        while(q != nullptr) {
            addTerm(q->exponent, q->coefficient);
            q = q->next;
        }
    }

    void purge() {
        Term *q = head;
        while(q != nullptr) {
            Term *p = q;
            q = q->next;
            delete p;
        }
    }

public:
    Polynomial() : head(nullptr) {}

    Polynomial(const Polynomial &p) : head(nullptr) { copy(p); }

    ~Polynomial() { purge(); }

    Polynomial & operator = (const Polynomial &p) { purge(); head = nullptr; copy(p); return *this; }

    void addTerm(int expon, int coeff) {
        if (head == nullptr) {
            Term *p = new Term(expon, coeff, nullptr);
            head = p;
            if (head->coefficient == 0) {
                Term *temp = head;
                head = nullptr;
                delete temp;
            }
        }
        else {
            Term *temp = head;
            if (expon > head->exponent) {
                Term *p = new Term(expon, coeff, head);
                head = p;
            }
            else if (expon == head->exponent) {
                head->coefficient += coeff;
                if (head->coefficient == 0) {
                    head = temp->next;
                    delete temp;
                }
            }
            else {
                Term *help = temp->next;
                while (temp != nullptr) {
                    if (temp->next != nullptr) {
                        if (expon > help->exponent) {
                            Term *t = new Term (expon, coeff, help);
                            temp->next = t;
                            break;
                        }
                        else if (expon == help->exponent) {
                            help->coefficient += coeff;
                            if (help->coefficient == 0) {
                                Term *save = temp->next;
                                temp->next = help->next;
                                delete save;          
                                break;       
                            }
                            else {
                                break;
                            }
                        }
                        else {
                            temp = temp->next, help = help->next;
                        }
                    }
                    else {
                        Term *t = new Term (expon, coeff, nullptr);
                        temp->next = t;
                        break;
                    }
                }   
            }
        }
    }

    double evaluate(double x) {
        Term *p = head;
        double sum = 0;

        while(p != nullptr) {
            sum += (p->coefficient)*pow(x, p->exponent);
            p = p->next;
        }
        return sum;
    }

    friend Polynomial operator + (const Polynomial &p, const Polynomial &q) {
        Polynomial result;
        Term *i, *j;

        i = p.head;
        j = q.head;

        if (i == nullptr) {
            result.addTerm(j->exponent, j->coefficient);
            j = j->next;
        }
        else if (j == nullptr) {
            result.addTerm(i->exponent, i->coefficient);
            i = i->next;
        }
        else {
            while(i != nullptr && j != nullptr) {
                if (i->exponent > j->exponent) {
                    result.addTerm(i->exponent, i->coefficient);
                    i = i->next;
                }
                else if (i->exponent < j->exponent) {
                    result.addTerm(j->exponent, j->coefficient);
                    j = j->next;
                }
                else {
                    result.addTerm(i->exponent, i->coefficient + j->coefficient);
                    i = i->next;
                    j = j->next;
                }

            }
        }
        return result;
    }
 
    friend Polynomial operator * (const Polynomial &p, const Polynomial &q) {
        Polynomial result;
        Term *i, *j;

        for(i = p.head; i != nullptr; i = i->next) {
            for(j = q.head; j != nullptr; j = j->next) {
                result.addTerm(i->exponent + j->exponent, i->coefficient * j->coefficient);
            }
        }
        return result;
    }

    friend ostream & operator << (ostream &out, const Polynomial &p) {
        Term *n = p.head;

        if (n == nullptr) {
            out << "0";
            return out;
        }

        if (n->coefficient < 0) {
            out << "- ";
        }
        if (abs(n->coefficient) != 1) {
            out << abs(n->coefficient);
        }
        if (n->exponent == 1 && abs(n->coefficient) == 1) {
            out << "1";
        }
        if (n->exponent == 1) {
            out << "x";
        }
        else if (n->exponent != 0) {
            out << "x^" << n->exponent;
        }
        n = n->next;
        while (n != nullptr) {
            if (n->coefficient < 0) {
                out << " - ";
            }
            else {
                out << " + ";
            }
            if(abs(n->coefficient) != 1 || n->exponent == 0) {
                out << abs(n->coefficient);
            }
            if (n->exponent == 1) {
                out << "x";
            }
            else if (n->exponent != 0) {
                out << "x^" << n->exponent;
            }
            n = n->next;
        }
        return out;
    } 
};

int main () {
    Polynomial p;
    p.addTerm(1, 3);
    p.addTerm(2, 1);
    p.addTerm(0, -1);
    Polynomial q(p);
    q.addTerm(1, -3);
    q.addTerm(3, -12);
    cout << "P(X) = " << p << endl;
    cout << "P(1) = " << p.evaluate(1) << endl;
    cout << "Q(X) = " << q << endl;
    cout << "Q(1) = " << q.evaluate(1) << endl;
    cout << "(P+Q)(X) = " << p+q << endl;
    cout << "(P*Q)(X) = " << p*q << endl;
}