//24BCS11540 (Krissh Chhabra)

#include <iostream>
using namespace std;

class Node
{   
    public:
    int data;
    Node *next;
};

class LinkedList
{
    private:
    Node *head;
    
    public:
    LinkedList()
    {
        head = nullptr;
    }

    void insertAtBeginning(int val)
    {

        if (head == nullptr)
        {
            head = new Node();
            head->data = val;
            head->next = nullptr;
            return;
        }

        Node *newNode = new Node();
        newNode->data = val;
        newNode->next = head;
        head = newNode;
    }

    void insertAtEnd(int val)
    {
        Node *newNode = new Node();
        newNode -> data = val;
        newNode -> next = nullptr;

        Node *temp = head;
        while (temp != nullptr && temp->next != nullptr)
        {
            temp = temp -> next;
        }
        temp->next = newNode;
    }

    void deleteFromBeginning()
    {
        if (head == nullptr)
        {
            cout << "List is empty, nothing to delete." << endl;
            return;
        }

        Node *temp = head;
        head = head -> next;
        delete temp;
    }

    void deleteFromEnd()
    {
        Node *temp = head;
        if (temp == nullptr)
        {
            cout << "List is already empty." << endl;
            return;
        }

        if (temp -> next == nullptr)
        {
            delete temp;
            head = nullptr;
            return;
        }

        while (temp -> next -> next != nullptr)
        {
            temp = temp -> next;
        }
        delete temp -> next;
        temp -> next = nullptr;
    }

    void deleteGivenValue(int val)
    {
        if (head == nullptr)
        {
            cout << "List is already empty." << endl;
            return;
        }

        Node *temp = head;
        if (temp -> data == val)
        {
            head = head -> next;
            delete temp;
            return;
        }

        while (temp -> next != nullptr && temp -> next -> data != val)
        {
            temp = temp -> next;
        }

        if (temp -> next == nullptr)
        {
            cout << "No such value found in the List." << endl;
            return;
        }

        Node *nodeToDelete = temp -> next;
        temp -> next = temp -> next -> next;
        delete nodeToDelete;
    }

    void display()
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            cout << temp -> data << " ";
            temp = temp -> next;
        }
    }

};

int main()
{
    LinkedList obj;
    obj.insertAtBeginning(10);
    obj.insertAtBeginning(20);
    obj.insertAtEnd(30);
    obj.insertAtEnd(40);
    cout << "Linked List elements: ";
    obj.display();
    obj.deleteFromEnd();
    obj.deleteFromBeginning();
    obj.deleteGivenValue(30);
    cout << "\nLinked List elements after deletions: ";
    obj.display();
    return 0;
}