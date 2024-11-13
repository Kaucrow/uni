-- 1. Display the average grade of a major's students
SELECT SUM(subj.grade)/COUNT(subj.grade) average
FROM assc.student_subjects subj
LEFT JOIN people.students stdt ON subj.student_id=stdt.student_id
WHERE passed=true AND stdt.major_id=0;

-- 2. Get the subjects passed by a student and their trimester
SELECT subj.subject_name, subjs.grade, subjs.trimester, stdt.first_name, stdt.last_name
FROM assc.student_subjects subjs
LEFT JOIN people.students stdt ON subjs.student_id=stdt.student_id
LEFT JOIN offers.subjects subj ON subjs.subject_id=subj.subject_id
WHERE subjs.passed=true AND subjs.student_id=0;

-- 3. Get the subjects taught by a teacher in a trimester
SELECT subj.subject_name, teach.first_name, teach.last_name, subj.trimester
FROM assc.teacher_subjects subjs
LEFT JOIN people.teachers teach ON subjs.teacher_id=teach.teacher_id
LEFT JOIN offers.subjects subj ON subjs.subject_id=subj.subject_id
WHERE teach.teacher_id=1 AND subj.trimester=11;

-- 4. Obtain, in a single table, the amount of subjects taken, the hc total, the amount
--    of subjects passed, and the average grade of a student

-- Without view
SELECT
    stdt.first_name,
    stdt.last_name,
    COUNT(subjs.subject_id) subjects_taken,
    COUNT(1) FILTER (WHERE subjs.passed=true) subjects_passed,
    SUM(subj.num_hc) FILTER (WHERE subjs.passed=true) total_hc,
    SUM(subjs.grade)/COUNT(subjs.subject_id) FILTER (WHERE subjs.passed=true) average_grade
FROM assc.student_subjects subjs
LEFT JOIN offers.subjects subj ON subjs.subject_id=subj.subject_id
LEFT JOIN people.students stdt ON subjs.student_id=stdt.student_id
GROUP BY subjs.student_id, stdt.first_name, stdt.last_name;

-- With view
SELECT * FROM req_4;

-- 5. Display the amount of subjects, amount of total hc, weekly pay, and level of a
--    teacher in a trimester

SELECT
    teach.first_name,
    teach.last_name,
    teach.teacher_type type,
    COUNT(subjs.subject_id)subject_amt,
    SUM(subj.num_hc) hc_amt,
    SUM(subj.num_hc * teach.dollars_per_hc) weekly_pay_usd,
    teach.dollars_per_hc usd_per_hc
FROM assc.teacher_subjects subjs
LEFT JOIN people.teachers teach ON subjs.teacher_id=teach.teacher_id
LEFT JOIN offers.subjects subj ON subjs.subject_id=subj.subject_id
GROUP BY teach.first_name, teach.last_name, teach.dollars_per_hc, teach.teacher_type;