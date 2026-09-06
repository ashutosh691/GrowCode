create database if not exists growcode;

use growcode;

/*
this is user table schema 
username and email are need to be unique and not null always
password is going to be stored in a safer manner so we will use hashing algo 
    if input password hash = stored hash value in the database then only login happens
this table stores user data
*/
create table users(
    user_id int auto_increment primary key, username varchar(50) not null unique,
    email varchar(100) not null unique, hash_password varchar(50) not null,
    created_at timestamp default current_timestamp  
);

/*
this is the problem set table inside which we will store problem content
time limit will be in ms 
memory limit will be in mb
*/
create table problems (
    problem_id int auto_increment primary key,
    title varchar(150) not null, description text not null,
    difficulty enum('easy', 'medium', 'hard') not null,
    time_limit int not null default 1000, memory_limit int not null default 256,
    created_at timestamp default current_timestamp
);

/* 
this is test case table where test cases are stored in realtion to a problem
here foreign key is problem_id as every test case is related to a problem id
*/
create table test_cases (
    test_case_id int auto_increment primary key,
    problem_id int not null,
    input_data text not null,
    expected_output text not null,
    is_sample boolean default false,
    foreign key (problem_id) references problems(problem_id) on delete cascade
    /*delete cascade is used here to solve the problem when any problem is deleted or removes it does not makes an error and all test cases related to that problem will also get removed */
);

/* this is submission table 
this stores data for user submission
2 foreign keys are used here, 1 from users table and 1 from problems table
*/
create table submissions (
    submission_id int auto_increment primary key,
    user_id int not null,
    problem_id int not null,
    source_code text not null,
    language varchar(20) not null default 'c++',
    submitted_at timestamp default current_timestamp,

    foreign key (user_id) references users(user_id) on delete cascade,
    foreign key (problem_id) references problems(problem_id) on delete cascade
);

/* this table is for execution results 
it stores the verdict of every submission
it stores all important info such as verdict, message */
create table execution_results (
    result_id int auto_increment primary key,
    submission_id int not null,
    verdict enum('pending', 'accepted', 'wrong answer', 'compilation error',
        'runtime error', 'time limit exceeded', 'memory limit exceeded') not null default 'pending',
    execution_time_ms int, memory_used_kb int,
    error_message text,
    executed_at timestamp null,

    foreign key (submission_id) references submissions(submission_id) on delete cascade
);

/* this is user progress table
this determines which user has solved which q by checking all possible combination of user_id and problem_id
*/
create table user_progress (
    progress_id int auto_increment primary key, user_id int not null,
    problem_id int not null, attempts int not null default 0,
    solved boolean not null default false, last_submission_id int null,
    updated_at timestamp default current_timestamp on update current_timestamp,

    foreign key (user_id) references users(user_id) on delete cascade,
    foreign key (problem_id) references problems(problem_id) on delete cascade,
    foreign key (last_submission_id) references submissions(submission_id) on delete set null,
    unique (user_id, problem_id) /*combination is req to be unique*/
);
