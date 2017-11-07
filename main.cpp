#include <QCoreApplication>
#include <string>
#include <iostream>
#include <fstream>>
#include <sstream>>
#include <iomanip>
#include <vector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QString>
#include <QVariant>

using namespace std;

//splits a string separated by a certain character and stores each field in a vector
vector<string> splitString(string text, char sep)
{
    string field;
    vector<string> fields;

    //string to split
    istringstream inputStr(text);

    //get the fields from the input string
    while (getline(inputStr, field, sep))
    {
        //add it to the vector
        fields.push_back(field);
    }
    return fields;
}

int main()
{
    string line;
    //input file
    ifstream input("/Users/ckimb/OneDrive/Documents/CS245-Lab5-BulkInsert/Customers.txt");
    vector<string> fields;

    //check to see if the file opens
    if(input.fail())
    {
        cout << "File not found. Exiting..." << endl;
        return 1;
    }

    //open database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={SQL Server};Server=customerapp-ck.database.windows.net;Database=restaurant;Uid=cs245@customerapp-ck;Port=1433;Pwd=Thomas123;encrypt=true;trustServerCertificate=false;hostNameInCertificate=*.database.windows.net;loginTimeout=30;");

    //check to see if the database connection opened properly
    bool ok = db.open();
    if(ok)
    {
        //if it did, loop through the file until its end of file character
        while(!input.eof())
        {
            cout << "Adding data to DataBase" << endl;
            //get the current line of the file
            getline(input, line);
            //split it by tabs
            fields = splitString(line, '\t');


            if(fields.size() == 5)
            {
                //get the fields for the database table
                string firstName = fields[0];
                string lastName = fields[1];
                string email = fields[2];
                string city = fields[3];
                string state = fields[4];

                //build string for SQL statement
                QString sql;
                sql += "INSERT INTO customer(firstName, lastName, email, city, state) ";
                sql += "VALUES (:firstName, :lastName, :email, :city, :state)";

                //build prepared SQL statement
                QSqlQuery query;
                query.setForwardOnly(true);
                query.prepare(sql);

                //bind values
                query.bindValue(0, QString::fromStdString(firstName));
                query.bindValue(1, QString::fromStdString(lastName));
                query.bindValue(2, QString::fromStdString(email));
                query.bindValue(3, QString::fromStdString(city));
                query.bindValue(4, QString::fromStdString(state));

                //execute SQL statement
                query.exec();
            }
        }
        cout << "Data added successfully" << endl;
    }
    else
    {
        cout << "Could not connect" << endl;
    }
    cout << "Closing file and db connection" << endl;
    //clean-up
    //close file and database connection
    input.close();
    db.close();
    return 0;
}
