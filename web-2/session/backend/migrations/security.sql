CREATE TABLE IF NOT EXISTS security.user (
    user_id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    email VARCHAR(50) UNIQUE NOT NULL,
    passwd TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS security.profile (
    profile_id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    profile_name VARCHAR(50) UNIQUE NOT NULL
);

CREATE TABLE IF NOT EXISTS security.user_profile (
    user_id UUID NOT NULL REFERENCES security.user(user_id),
    profile_id UUID NOT NULL REFERENCES security.profile(profile_id)
);

CREATE TABLE IF NOT EXISTS security.module (

);

CREATE TABLE IF NOT EXISTS security.object (

);

CREATE TABLE IF NOT EXISTS security.method (

);

CREATE TABLE IF NOT EXISTS security.method_permission (

);

CREATE TABLE IF NOT EXISTS security.option (

);

CREATE TABLE IF NOT EXISTS security.permission_option (

);