#include <iostream>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class DatabaseConnect {
public:
  virtual bool login(std::string username, std::string passwrod) {return true;}
  virtual bool logout(std::string username) {return true;}
  virtual int fetchRecord() {return -1;}
};

class MockDB : public DatabaseConnect {
public:
  MOCK_METHOD0(fetchRecord, int());
  MOCK_METHOD1(logout, bool (std::string username));
  MOCK_METHOD2(login, bool (std::string username, std::string password));
};

class MyDatabase {
  DatabaseConnect & dbC;
public: 
  MyDatabase(DatabaseConnect& dbC): dbC(dbC) {}
  int Init(std::string username, std::string password) {
    if (dbC.login(username, password) != true) {
      std::cout << "DB Failure" << std::endl; return -1;
    } else {
      std::cout << "DB Success" << std::endl; return 1;
    }
  }
};

TEST (MyDBTest, LoginTest) {
  // Arrange
  MockDB mdb;
  MyDatabase db(mdb);
  EXPECT_CALL(mdb, login("Terminator", _)).
    Times(AtLeast(1)).
    WillOnce(Return(true));

  int retValue = db.Init("Terminator", "I'm Back");

  EXPECT_EQ(retValue, 1);
}

int main(int argc, char** argv) {
  
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();

}
