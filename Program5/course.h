#include <string>

// days of the weekday for class
enum Days {None = 0, Monday=1, Tuesday=2, Wednesday=3, Thursday=4, Friday=5};

// course name and id 
struct Course
{
    std::string courseName;
    std::string courseID;
};

// misc info about course
struct CourseInfo
{
    Course course;
    std::string meetingDays;
    std::string meetingTimes;
    std::string building;
    std::string room;
    CourseInfo *next;
};