"""
StudentInfo(StudentID, name, pwd, dept, totalScore, compulsoryScore, totalGPA, compulsoryGPA)
TeacherInfo(teacherID, name, pwd)
CourseInfo(courseID, name, score, teacherID, descript)
Compulsory(dept, course)
CourseSelect(studentID, courseID, grade)
Notice(ID, teacherID, text, timestamp)
"""
from decimal import Decimal


def getGPA(score):
    if score >= 90:
        return 4.0
    if score >= 85:
        return 3.7
    if score >= 80:
        return 3.3
    if score >= 76:
        return 3.0
    if score >= 73:
        return 2.7
    if score >= 70:
        return 2.3
    if score >= 66:
        return 2.0
    if score >= 63:
        return 1.7
    if score >= 61:
        return 1.3
    if score == 60:
        return 1.0
    return 0


def GPA(score):
    return Decimal(getGPA(score))


def hashcode(word: str):
    hashc = 0
    for i in range(len(word)):
        hashc = hashc + (i+1) * ord(word[i])
    return (hashc+14507) % 100007
