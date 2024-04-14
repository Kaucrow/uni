CREATE VIEW req_4 AS
SELECT
    stdt.student_id,
    stdt.first_name,
    stdt.last_name,
    COUNT(subjs.subject_id) subjects_taken,
    COUNT(1) FILTER (WHERE subjs.passed=true) subjects_passed,
    SUM(subj.num_hc) FILTER (WHERE subjs.passed=true) total_hc,
    SUM(subjs.grade)/COUNT(subjs.subject_id) FILTER (WHERE subjs.passed=true) average_grade
FROM assc.student_subjects subjs
LEFT JOIN offers.subjects subj ON subjs.subject_id=subj.subject_id
LEFT JOIN people.students stdt ON subjs.student_id=stdt.student_id
GROUP BY subjs.student_id, stdt.first_name, stdt.last_name, stdt.student_id;