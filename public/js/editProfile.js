// Edit Profile JavaScript Functionality

// DOM Elements
const editProfileForm = document.getElementById("editProfileForm");
const profilePhotoPreview = document.getElementById("profilePhotoPreview");
const profilePhotoInput = document.getElementById("profilePhotoInput");
const changePhotoBtn = document.getElementById("changePhotoBtn");
const closeModal = document.getElementById("closeModal");
const saveSettings = document.getElementById("saveSettings");
const resetToDefault = document.getElementById("resetToDefault");
const messageContainer = document.getElementById("messageContainer");
const successMessage = document.getElementById("successMessage");
const errorMessage = document.getElementById("errorMessage");
const errorText = document.getElementById("errorText");

// Form inputs
const fullNameInput = document.getElementById("fullName");
const emailAddressInput = document.getElementById("emailAddress");
const phoneNumberInput = document.getElementById("phoneNumber");
const addressInput = document.getElementById("address");
const bioInput = document.getElementById("bio");

// Default values
const defaultValues = {
  fullName: "Admin User",
  emailAddress: "admin@example.com",
  phoneNumber: "+62 812-3456-7890",
  address: "Jakarta, Indonesia",
  bio: "System Administrator with 5+ years of experience in managing IT infrastructure and web applications.",
  profilePhoto: null,
};

// Current profile photo
let currentProfilePhoto = null;

// Utility Functions
function showMessage(type, message) {
  messageContainer.classList.remove("hidden");

  if (type === "success") {
    successMessage.classList.remove("hidden");
    errorMessage.classList.add("hidden");
  } else {
    errorMessage.classList.remove("hidden");
    successMessage.classList.add("hidden");
    errorText.textContent = message;
  }

  // Auto hide after 5 seconds
  setTimeout(() => {
    hideMessages();
  }, 5000);
}

function hideMessages() {
  messageContainer.classList.add("hidden");
  successMessage.classList.add("hidden");
  errorMessage.classList.add("hidden");
}

function isValidEmail(email) {
  const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  return emailRegex.test(email);
}

function isValidPhone(phone) {
  const phoneRegex = /^[\+]?[0-9\s\-\(\)]{10,}$/;
  return phoneRegex.test(phone);
}

function validateForm() {
  const formData = getFormData();
  let errors = [];

  // Full name validation
  if (!formData.fullName.trim()) {
    errors.push("Full name is required");
  } else if (formData.fullName.trim().length < 2) {
    errors.push("Full name must be at least 2 characters");
  }

  // Email validation
  if (!formData.emailAddress.trim()) {
    errors.push("Email address is required");
  } else if (!isValidEmail(formData.emailAddress)) {
    errors.push("Please enter a valid email address");
  }

  // Phone validation
  if (formData.phoneNumber.trim() && !isValidPhone(formData.phoneNumber)) {
    errors.push("Please enter a valid phone number");
  }

  return errors;
}

function getFormData() {
  return {
    fullName: fullNameInput.value,
    emailAddress: emailAddressInput.value,
    phoneNumber: phoneNumberInput.value,
    address: addressInput.value,
    bio: bioInput.value,
    profilePhoto: currentProfilePhoto,
  };
}

function setFormData(data) {
  fullNameInput.value = data.fullName || "";
  emailAddressInput.value = data.emailAddress || "";
  phoneNumberInput.value = data.phoneNumber || "";
  addressInput.value = data.address || "";
  bioInput.value = data.bio || "";
}

function resetProfilePhoto() {
  currentProfilePhoto = null;
  profilePhotoPreview.innerHTML = `
        <svg class="w-16 h-16 text-gray-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M16 7a4 4 0 11-8 0 4 4 0 018 0zM12 14a7 7 0 00-7 7h14a7 7 0 00-7-7z"></path>
        </svg>
    `;
}

// Event Listeners

// Change photo button
changePhotoBtn.addEventListener("click", () => {
  profilePhotoInput.click();
});

// Profile photo input change
profilePhotoInput.addEventListener("change", (e) => {
  const file = e.target.files[0];

  if (file) {
    // Validate file type
    const validTypes = ["image/jpeg", "image/jpg", "image/png", "image/gif"];
    if (!validTypes.includes(file.type)) {
      showMessage("error", "Please upload a valid image file (JPG, PNG, GIF)");
      return;
    }

    // Validate file size (2MB max)
    const maxSize = 2 * 1024 * 1024; // 2MB in bytes
    if (file.size > maxSize) {
      showMessage("error", "File size must be less than 2MB");
      return;
    }

    // Create FileReader to preview image
    const reader = new FileReader();
    reader.onload = (e) => {
      currentProfilePhoto = e.target.result;
      profilePhotoPreview.innerHTML = `
                <img src="${e.target.result}" alt="Profile Photo" class="w-full h-full object-cover rounded-full">
            `;
    };
    reader.readAsDataURL(file);
  }
});

// Close modal
closeModal.addEventListener("click", () => {
  if (window.parent && window.parent.closeEditProfileModal) {
    window.parent.closeEditProfileModal();
  } else {
    window.close();
  }
});

// Reset to default
resetToDefault.addEventListener("click", () => {
  const confirmReset = confirm(
    "Are you sure you want to reset all fields to default values?"
  );

  if (confirmReset) {
    setFormData(defaultValues);
    resetProfilePhoto();
    showMessage("success", "Profile has been reset to default values");
  }
});

// Form submission
editProfileForm.addEventListener("submit", (e) => {
  e.preventDefault();
  handleSaveProfile();
});

// Save settings button
saveSettings.addEventListener("click", (e) => {
  e.preventDefault();
  handleSaveProfile();
});

// Handle save profile
async function handleSaveProfile() {
  hideMessages();

  const errors = validateForm();
  if (errors.length > 0) {
    showMessage("error", errors[0]);
    return;
  }

  const formData = getFormData();

  try {
    // Show loading state
    saveSettings.disabled = true;
    saveSettings.innerHTML = `
            <svg class="animate-spin -ml-1 mr-2 h-5 w-5 text-white" xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24">
                <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
            </svg>
            Saving...
        `;

    // Simulate API call
    await new Promise((resolve) => setTimeout(resolve, 1500));

    // Save to localStorage (simulate saving to backend)
    localStorage.setItem("userProfile", JSON.stringify(formData));

    showMessage("success", "Profile updated successfully!");

    // Update parent window if available
    if (window.parent && window.parent.updateProfileData) {
      window.parent.updateProfileData(formData);
    }
  } catch (error) {
    showMessage("error", "Error updating profile. Please try again.");
  } finally {
    // Reset button state
    saveSettings.disabled = false;
    saveSettings.innerHTML = `
            <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7H5a2 2 0 00-2 2v9a2 2 0 002 2h14a2 2 0 002-2V9a2 2 0 00-2-2h-3m-1 4l-3 3m0 0l-3-3m3 3V4"></path>
            </svg>
            Save Settings
        `;
  }
}

// Load saved profile data
function loadProfileData() {
  const savedProfile = localStorage.getItem("userProfile");

  if (savedProfile) {
    try {
      const profileData = JSON.parse(savedProfile);
      setFormData(profileData);

      if (profileData.profilePhoto) {
        currentProfilePhoto = profileData.profilePhoto;
        profilePhotoPreview.innerHTML = `
                    <img src="${profileData.profilePhoto}" alt="Profile Photo" class="w-full h-full object-cover rounded-full">
                `;
      }
    } catch (error) {
      console.error("Error loading profile data:", error);
    }
  }
}

// Input validation and filtering
function setupInputValidation() {
  // Full name - only letters and spaces
  fullNameInput.addEventListener("input", (e) => {
    let value = e.target.value;
    // Allow letters, spaces, and common name characters
    value = value.replace(/[^a-zA-Z\s\-\.\']/g, "");
    if (value !== e.target.value) {
      e.target.value = value;
    }
  });

  // Phone number - only numbers, spaces, +, -, (, )
  phoneNumberInput.addEventListener("input", (e) => {
    let value = e.target.value;
    value = value.replace(/[^0-9\s\+\-\(\)]/g, "");
    if (value !== e.target.value) {
      e.target.value = value;
    }
  });
}

// Keyboard shortcuts
document.addEventListener("keydown", (e) => {
  // Ctrl/Cmd + S to save
  if ((e.ctrlKey || e.metaKey) && e.key === "s") {
    e.preventDefault();
    handleSaveProfile();
  }

  // Escape to close
  if (e.key === "Escape") {
    closeModal.click();
  }
});

// Initialize
document.addEventListener("DOMContentLoaded", () => {
  console.log("Edit Profile form initialized");

  // Load existing profile data
  loadProfileData();

  // Setup input validation
  setupInputValidation();

  // Focus on first input
  fullNameInput.focus();

  // Add entrance animation delay
  setTimeout(() => {
    document.getElementById("editProfileModal").style.animationDelay = "0.1s";
  }, 100);
});
