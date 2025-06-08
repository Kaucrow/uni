import './Profile.css';
import { useState } from 'react';
import { useAuth } from '../../hooks/useAuth';
import {
  doPasswordChange,
  doSignOut,
  doDeleteAccount,
  doReauthenticateWithCredential,
} from '../../firebase/auth';
import { EmailAuthProvider } from 'firebase/auth';
import CenteredDiv from '../../components/CenteredDiv';
import Input from '../../components/Input';
import { Navigate } from 'react-router-dom';

const Profile = () => {
  const { currentUser, userLoggedIn } = useAuth();
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [success, setSuccess] = useState('');

  // Password change
  const [currentPassword, setCurrentPassword] = useState('');
  const [newPassword, setNewPassword] = useState('');

  // Delete account confirmation
  const [showDeleteConfirmation, setShowDeleteConfirmation] = useState(false);

  const handlePasswordChange = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    setError('');
    setSuccess('');

    try {
      // Reauthenticate user before password change
      const credential = EmailAuthProvider.credential(
        currentUser?.email || '',
        currentPassword
      );
      await doReauthenticateWithCredential(currentUser, credential);

      await doPasswordChange(newPassword);
      setSuccess('Password updated successfully!');
      setCurrentPassword('');
      setNewPassword('');
    } catch (err) {
      setError((err as Error).message || 'Failed to update password.');
    } finally {
      setLoading(false);
    }
  };

  const handleDeleteAccount = async () => {
    setLoading(true);
    setError('');
    
    try {
      await doDeleteAccount(currentUser);
      // Redirect happens automatically via auth state change
    } catch (err) {
      setError((err as Error).message || 'Failed to delete account.');
      setLoading(false);
      setShowDeleteConfirmation(false);
    }
  };

  if (!userLoggedIn) {
    //return <div className="profile-container">Please log in to view this page.</div>;
    return <Navigate to={'/'} replace />
  }

  return (
    <CenteredDiv className="profile-container">
      <div className="profile-header">
        <p className="profile-header-title">User Profile</p>
        <div className="user-info">
          <p><strong>Email:</strong> {currentUser?.email}</p>
          <p><strong>Email Verified:</strong> {currentUser?.emailVerified ? '✅' : '❌'}</p>
        </div>
      </div>

      {/* Change Password Section */}
      <div className="profile-section">
        <p className="subtitle">Change Password</p>
        <form onSubmit={handlePasswordChange}>
          <div className="form-group">
            <Input
              required
              id="current-password-input"
              input={currentPassword}
              setInput={setCurrentPassword}
              label="Current Password"
            />
          </div>
          <div className="form-group">
            <Input
              required
              id="new-password-input"
              input={newPassword}
              setInput={setNewPassword}
              label="New Password"
            />
          </div>
          <button type="submit" className="update-password-btn" disabled={loading}>
            {loading ? 'Updating...' : 'Update Password'}
          </button>
          {error && <p className="error">{error}</p>}
          {success && <p className="success">{success}</p>}
        </form>
      </div>

      {/* Account Actions Section */}
      <div className="profile-section">
        <p className="subtitle">Account Actions</p>
        <div className="action-buttons">
          <button
            onClick={doSignOut}
            className="logout-btn"
          >
            Log Out
          </button>
          <button
            onClick={() => setShowDeleteConfirmation(true)}
            className="delete-btn"
          >
            Delete Account
          </button>
        </div>
      </div>

      {/* Delete Account Confirmation Modal */}
      {showDeleteConfirmation && (
        <div className="modal-overlay">
          <div className="modal">
            <h3>Confirm Account Deletion</h3>
            <p>This action cannot be undone. All your data will be permanently deleted.</p>
            <div className="modal-buttons">
              <button
                onClick={() => setShowDeleteConfirmation(false)}
                disabled={loading}
              >
                Cancel
              </button>
              <button
                onClick={handleDeleteAccount}
                disabled={loading}
                className="delete-btn"
              >
                {loading ? 'Deleting...' : 'Delete Forever'}
              </button>
            </div>
          </div>
        </div>
      )}
    </CenteredDiv>
  );
};

export default Profile;