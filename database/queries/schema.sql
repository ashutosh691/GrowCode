create database if not exists growcode;

use growcode;

/*
this is user table schema 
username and email are need to be unique and not null always
password is going to be stored in a safer manner so we will use hashing algo 
    if input password hash = stored hash value in the database then only login happens

*/
create table users(
    user_id int auto_increment primary key, username varchar(50) not null unique,
    email varchar(100) not null unique, hash_password varchar(50) not null,
    created_at timestamp default current_timestamp  
);


