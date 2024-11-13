CREATE SCHEMA IF NOT EXISTS offers;

CREATE TABLE IF NOT EXISTS offers.majors (
    major_id SMALLSERIAL PRIMARY KEY,
    major_name VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS offers.subjects (
    subject_id SERIAL PRIMARY KEY,
    subject_name VARCHAR(255) NOT NULL,
    trimester SMALLINT NOT NULL,
    num_hc SMALLINT NOT NULL,
    CHECK (num_hc>=0),
    CHECK (trimester>=1)
);

CREATE SCHEMA IF NOT EXISTS people;

CREATE TABLE IF NOT EXISTS people.teachers (
    teacher_id SERIAL PRIMARY KEY,
    major_id SMALLINT NOT NULL,
    dollars_per_hc SMALLINT NOT NULL,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    teacher_type VARCHAR(255) NOT NULL,
    FOREIGN KEY (major_id) REFERENCES offers.majors(major_id),
    CHECK (dollars_per_hc>=1)
);

CREATE TABLE IF NOT EXISTS people.students (
    student_id SERIAL PRIMARY KEY,
    major_id SMALLINT NOT NULL,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    FOREIGN KEY (major_id) REFERENCES offers.majors(major_id)
);

CREATE SCHEMA IF NOT EXISTS assc;

CREATE TABLE IF NOT EXISTS assc.teacher_subjects (
    teacher_id INT NOT NULL,
    subject_id INT NOT NULL UNIQUE,
    FOREIGN KEY (teacher_id) REFERENCES people.teachers(teacher_id),
    FOREIGN KEY (subject_id) REFERENCES offers.subjects(subject_id)
);

CREATE TABLE IF NOT EXISTS assc.student_subjects (
    subject_id INT NOT NULL,
    student_id INT NOT NULL,
    passed BOOLEAN NOT NULL,
    trimester SMALLINT,
    grade NUMERIC(4,2),
    FOREIGN KEY (subject_id) REFERENCES offers.subjects(subject_id),
    FOREIGN KEY (student_id) REFERENCES people.students(student_id),
    CHECK (grade>=9.5 AND grade<=20.0)
);