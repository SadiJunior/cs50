from cs50 import SQL

roster_db = SQL("sqlite:///roster.db")
fixed_roster_db = SQL("sqlite:///roster_fixed.db")

houses = roster_db.execute("SELECT DISTINCT house, head FROM students ORDER BY house")

for house_row in houses:
    house = house_row["house"]
    head = house_row["head"]
    fixed_roster_db.execute("INSERT INTO houses (house, head) VALUES(?, ?)", house, head)

students = roster_db.execute("SELECT id, student_name, house FROM students")

for student_row in students:
    name = student_row["student_name"]
    student_id = student_row["id"]
    house_id = fixed_roster_db.execute("SELECT id FROM houses WHERE house = ?", student_row["house"])[0]["id"]
    fixed_roster_db.execute("INSERT INTO students (student_name) VALUES(?)", name)
    fixed_roster_db.execute("INSERT INTO assignments (student_id, house_id) VALUES(?, ?)", int(student_id), int(house_id))

