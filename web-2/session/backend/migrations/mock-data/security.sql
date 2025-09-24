-- Users
INSERT INTO security.user (email, passwd) VALUES
    ('user1@example.com', 'hashed_password_1'),
    ('user2@example.com', 'hashed_password_2'),
    ('user3@example.com', 'hashed_password_3')
ON CONFLICT (email) DO NOTHING;

-- Profiles
INSERT INTO security.profile (profile_name) VALUES
    ('admin'),
    ('moderator'),
    ('user'),
    ('viewer'),
    ('guest')
ON CONFLICT (profile_name) DO NOTHING;

-- Assign profiles to users
INSERT INTO security.user_profile (user_id, profile_id)
    SELECT u.user_id, p.profile_id 
    FROM security.user u, security.profile p 
    WHERE (u.email, p.profile_name) IN (
        ('user1@example.com', 'admin'),
        ('user1@example.com', 'user'),
        ('user2@example.com', 'moderator'),
        ('user2@example.com', 'viewer'),
        ('user3@example.com', 'user'),
        ('user3@example.com', 'guest')
    )
ON CONFLICT DO NOTHING;