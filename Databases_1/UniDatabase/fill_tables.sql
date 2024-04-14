-- MAJORS --

INSERT INTO offers.majors (major_id, major_name) VALUES
(0, 'Computer Eng.'),
(1, 'Electrical Eng.'),
(2, 'Telecomm Eng.'),
(3, 'Industrial Eng.'),
(4, 'Civil Eng.'),
(5, 'Chemical Eng.'),
(6, 'Psychology'),
(7, 'Accounting'),
(8, 'Architecture');

-- SUBJECTS --

INSERT INTO offers.subjects (subject_id, subject_name, trimester, num_hc) VALUES (0, 'Programming 1', 3, 3),
(1, 'Programming 2', 4, 3),
(2, 'Databases 1', 6, 5),
(3, 'Computer Architecture', 7, 4),
(4, 'Databases 2', 10, 3),
(5, 'AI Intro', 11, 2),
(6, 'Robotics Intro', 11, 2),
(7, 'Cryptography', 10, 2),
(8, 'Cybersecurity Intro', 9, 2),
(9, 'Visual Programming', 7, 5),
(10, 'Microcontrollers', 7, 4),
(11, 'Logical Design', 5, 5),
(12, 'Electronic Circuits', 4, 4),
(13, 'Networks 1', 8, 4),
(14, 'Electrical Circuits 1', 3, 2),
(15, 'Electrical Circuits 2', 4, 2),
(16, 'Electrical Machines', 5, 3),
(17, 'Electromagnetic Theory', 5, 3),
(18, 'Pipelines', 8, 2),
(19, 'Electronics 1', 5, 3);

-- TEACHERS --

INSERT INTO people.teachers (teacher_id, dollars_per_hc, major_id, first_name, last_name, teacher_type) VALUES
(0, 3, 0, 'Daniel', 'Carrizo', 'Adjunto'),
(1, 1, 0, 'Eli', 'Mora', 'Adjunto'),
(2, 5, 0, 'Genyelbert', 'Acosta', 'Adjunto'),
(3, 5, 1, 'German', 'Gutierrez', 'Adjunto'),
(4, 4, 1, 'Luis', 'Espina', 'Adjunto');

-- TEACHER'S SUBJECTS --

INSERT INTO assc.teacher_subjects (teacher_id, subject_id) VALUES
(0, 2),
(0, 4),
(0, 9),
(0, 12),
(0, 13),
(1, 0),
(1, 1),
(1, 5),
(1, 6),
(1, 7),
(1, 8),
(2, 3),
(2, 10),
(2, 11),
(3, 18),
(4, 14),
(4, 15),
(4, 16),
(4, 17),
(4, 19);

-- STUDENTS --

INSERT INTO people.students (student_id, major_id, first_name, last_name) VALUES
(0, 1, 'Geanfranco', 'Piccioni'),
(1, 0, 'Rebecca', 'Bracho'),
(2, 0, 'David', 'Esber'),
(3, 1, 'Angel', 'Mipana'),
(4, 1, 'Juan', 'Camarillo'),
(5, 0, 'Jesus', 'Melendez'),
(6, 0, 'Ramon', 'Alvarez');


-- STUDENTS' SUBJECTS --

INSERT INTO assc.student_subjects (student_id, subject_id, passed, trimester, grade) VALUES
(0, 14, false, NULL, NULL),
(0, 15, true, 1, 12.3),
(0, 16, false, NULL, NULL),
(0, 17, true, 3, 17.3),
(0, 18, false, NULL, NULL),
(0, 19, true, 8, 19.1),
(1, 3, false, NULL, NULL),
(1, 4, true, 9, 11.8),
(1, 5, false, NULL, NULL),
(2, 3, true, 10, 14.2),
(2, 6, false, NULL, NULL),
(2, 7, true, 2, 17.6),
(2, 9, false, NULL, NULL),
(3, 18, true, 4, 20.0),
(3, 19, false, NULL, NULL),
(4, 11, true, 6, 17.6),
(4, 14, false, NULL, NULL),
(4, 17, true, 5, 16.8),
(5, 2, false, NULL, NULl),
(5, 3, true, 7, 18.6),
(5, 4, false, NULL, NULL),
(6, 3, true, 8, 10.6),
(6, 9, false, NULL, NULL),
(6, 10, true, 2, 15.1),
(6, 12, false, NULL, NULL),
(6, 13, true, 11, 18.0);