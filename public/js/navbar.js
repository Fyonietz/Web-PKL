// Navbar JavaScript Functionality

// State management
let currentDropdown = null;

// DOM Elements
const accountDropdownBtn = document.getElementById("accountDropdownBtn");
const accountDropdown = document.getElementById("accountDropdown");
const accountDropdownArrow = document.getElementById("accountDropdownArrow");

const companyDropdownBtn = document.getElementById("companyDropdownBtn");
const companyDropdown = document.getElementById("companyDropdown");
const companyDropdownArrow = document.getElementById("companyDropdownArrow");

const profileDropdownBtn = document.getElementById("profileDropdownBtn");
const profileDropdown = document.getElementById("profileDropdown");
const profileDropdownArrow = document.getElementById("profileDropdownArrow");

const mobileMenuBtn = document.getElementById("mobileMenuBtn");
const mobileMenu = document.getElementById("mobileMenu");
const mobileMenuIcon = document.getElementById("mobileMenuIcon");
const mobileCloseIcon = document.getElementById("mobileCloseIcon");
const mobileOverlay = document.getElementById("mobileOverlay");

// Mobile dropdowns
const mobileAccountBtn = document.getElementById("mobileAccountBtn");
const mobileAccountDropdown = document.getElementById("mobileAccountDropdown");
const mobileAccountArrow = document.getElementById("mobileAccountArrow");

const mobileCompanyBtn = document.getElementById("mobileCompanyBtn");
const mobileCompanyDropdown = document.getElementById("mobileCompanyDropdown");
const mobileCompanyArrow = document.getElementById("mobileCompanyArrow");

// Status display
const statusDisplay = document.getElementById("statusDisplay");
const statusMessage = document.getElementById("statusMessage");

// Utility Functions
function closeAllDropdowns() {
  // Desktop dropdowns
  accountDropdown.classList.add("hidden");
  companyDropdown.classList.add("hidden");
  profileDropdown.classList.add("hidden");

  // Reset arrows
  accountDropdownArrow.style.transform = "";
  companyDropdownArrow.style.transform = "";
  profileDropdownArrow.style.transform = "";

  // Mobile dropdowns
  mobileAccountDropdown.classList.add("hidden");
  mobileCompanyDropdown.classList.add("hidden");
  mobileAccountArrow.style.transform = "";
  mobileCompanyArrow.style.transform = "";

  currentDropdown = null;
}

function toggleDropdown(dropdown, arrow, dropdownName) {
  if (currentDropdown === dropdownName) {
    // Close current dropdown
    dropdown.classList.add("hidden");
    arrow.style.transform = "";
    currentDropdown = null;
  } else {
    // Close all dropdowns first
    closeAllDropdowns();

    // Open selected dropdown
    dropdown.classList.remove("hidden");
    arrow.style.transform = "rotate(180deg)";
    currentDropdown = dropdownName;
  }
}

function showStatus(message, type = "info") {
  statusMessage.textContent = message;
  statusDisplay.classList.remove(
    "hidden",
    "bg-blue-50",
    "border-blue-200",
    "bg-green-50",
    "border-green-200",
    "bg-red-50",
    "border-red-200"
  );
  statusMessage.classList.remove(
    "text-blue-700",
    "text-green-700",
    "text-red-700"
  );

  switch (type) {
    case "success":
      statusDisplay.classList.add("bg-green-50", "border-green-200");
      statusMessage.classList.add("text-green-700");
      break;
    case "error":
      statusDisplay.classList.add("bg-red-50", "border-red-200");
      statusMessage.classList.add("text-red-700");
      break;
    default:
      statusDisplay.classList.add("bg-blue-50", "border-blue-200");
      statusMessage.classList.add("text-blue-700");
  }

  // Auto hide after 5 seconds
  setTimeout(() => {
    statusDisplay.classList.add("hidden");
  }, 5000);
}

// Event Listeners

// Desktop dropdown toggles
accountDropdownBtn.addEventListener("click", (e) => {
  e.stopPropagation();
  toggleDropdown(accountDropdown, accountDropdownArrow, "account");
});

companyDropdownBtn.addEventListener("click", (e) => {
  e.stopPropagation();
  toggleDropdown(companyDropdown, companyDropdownArrow, "company");
});

profileDropdownBtn.addEventListener("click", (e) => {
  e.stopPropagation();
  toggleDropdown(profileDropdown, profileDropdownArrow, "profile");
});

// Mobile menu toggle
mobileMenuBtn.addEventListener("click", () => {
  const isOpen = !mobileMenu.classList.contains("hidden");

  if (isOpen) {
    mobileMenu.classList.add("hidden");
    mobileOverlay.classList.add("hidden");
    mobileMenuIcon.classList.remove("hidden");
    mobileCloseIcon.classList.add("hidden");
    closeAllDropdowns();
  } else {
    mobileMenu.classList.remove("hidden");
    mobileOverlay.classList.remove("hidden");
    mobileMenuIcon.classList.add("hidden");
    mobileCloseIcon.classList.remove("hidden");
  }
});

// Mobile overlay click
mobileOverlay.addEventListener("click", () => {
  mobileMenu.classList.add("hidden");
  mobileOverlay.classList.add("hidden");
  mobileMenuIcon.classList.remove("hidden");
  mobileCloseIcon.classList.add("hidden");
  closeAllDropdowns();
});

// Mobile dropdown toggles
mobileAccountBtn.addEventListener("click", (e) => {
  e.stopPropagation();

  const isOpen = !mobileAccountDropdown.classList.contains("hidden");

  // Close company dropdown if open
  mobileCompanyDropdown.classList.add("hidden");
  mobileCompanyArrow.style.transform = "";

  if (isOpen) {
    mobileAccountDropdown.classList.add("hidden");
    mobileAccountArrow.style.transform = "";
  } else {
    mobileAccountDropdown.classList.remove("hidden");
    mobileAccountArrow.style.transform = "rotate(180deg)";
  }
});

mobileCompanyBtn.addEventListener("click", (e) => {
  e.stopPropagation();

  const isOpen = !mobileCompanyDropdown.classList.contains("hidden");

  // Close account dropdown if open
  mobileAccountDropdown.classList.add("hidden");
  mobileAccountArrow.style.transform = "";

  if (isOpen) {
    mobileCompanyDropdown.classList.add("hidden");
    mobileCompanyArrow.style.transform = "";
  } else {
    mobileCompanyDropdown.classList.remove("hidden");
    mobileCompanyArrow.style.transform = "rotate(180deg)";
  }
});

// Close dropdowns when clicking outside
document.addEventListener("click", (e) => {
  // Check if click is outside any dropdown
  if (!e.target.closest(".relative")) {
    closeAllDropdowns();
  }
});

// CRUD Action Handlers

// Account CRUD actions
function handleAccountAction(action) {
  closeAllDropdowns();

  // Close mobile menu if open
  mobileMenu.classList.add("hidden");
  mobileOverlay.classList.add("hidden");
  mobileMenuIcon.classList.remove("hidden");
  mobileCloseIcon.classList.add("hidden");

  let message = "";
  let type = "info";

  switch (action) {
    case "create":
      message = "Membuka form tambah akun baru...";
      type = "info";
      // TODO: Implement create account functionality
      console.log("Create Account Action");
      break;

    case "read":
      message = "Menampilkan daftar semua akun...";
      type = "info";
      // TODO: Implement read accounts functionality
      console.log("Read Accounts Action");
      break;

    case "update":
      message = "Membuka form edit akun...";
      type = "info";
      // TODO: Implement update account functionality
      console.log("Update Account Action");
      break;

    case "delete":
      message = "Menampilkan konfirmasi hapus akun...";
      type = "error";
      // TODO: Implement delete account functionality
      console.log("Delete Account Action");
      break;

    default:
      message = "Aksi akun tidak dikenali";
      type = "error";
  }

  showStatus(`Akun: ${message}`, type);
}

// Company CRUD actions
function handleCompanyAction(action) {
  closeAllDropdowns();

  // Close mobile menu if open
  mobileMenu.classList.add("hidden");
  mobileOverlay.classList.add("hidden");
  mobileMenuIcon.classList.remove("hidden");
  mobileCloseIcon.classList.add("hidden");

  let message = "";
  let type = "info";

  switch (action) {
    case "create":
      message = "Membuka form tambah perusahaan baru...";
      type = "info";
      // TODO: Implement create company functionality
      console.log("Create Company Action");
      break;

    case "read":
      message = "Menampilkan daftar semua perusahaan...";
      type = "info";
      // TODO: Implement read companies functionality
      console.log("Read Companies Action");
      break;

    case "update":
      message = "Membuka form edit perusahaan...";
      type = "info";
      // TODO: Implement update company functionality
      console.log("Update Company Action");
      break;

    case "delete":
      message = "Menampilkan konfirmasi hapus perusahaan...";
      type = "error";
      // TODO: Implement delete company functionality
      console.log("Delete Company Action");
      break;

    default:
      message = "Aksi perusahaan tidak dikenali";
      type = "error";
  }

  showStatus(`Perusahaan: ${message}`, type);
}

// Edit Profile Modal functionality
let editProfileModalOpen = false;

function openEditProfileModal() {
  closeAllDropdowns();

  // Close mobile menu if open
  mobileMenu.classList.add("hidden");
  mobileOverlay.classList.add("hidden");
  mobileMenuIcon.classList.remove("hidden");
  mobileCloseIcon.classList.add("hidden");

  // Create modal container
  const modalContainer = document.createElement("div");
  modalContainer.id = "editProfileModalContainer";
  modalContainer.className =
    "fixed inset-0 z-50 overflow-auto bg-black bg-opacity-50 flex items-center justify-center p-4";
  modalContainer.style.animation = "fadeIn 0.3s ease-out";

  // Create iframe for edit profile
  const iframe = document.createElement("iframe");
  iframe.src = "editProfile.html";
  iframe.className =
    "w-full max-w-4xl h-[90vh] max-h-[800px] bg-white rounded-lg shadow-xl";
  iframe.style.animation = "slideUp 0.3s ease-out";
  iframe.style.border = "none";

  modalContainer.appendChild(iframe);
  document.body.appendChild(modalContainer);

  // Prevent body scroll
  document.body.style.overflow = "hidden";

  editProfileModalOpen = true;

  // Close modal when clicking outside iframe
  modalContainer.addEventListener("click", function (e) {
    if (e.target === modalContainer) {
      closeEditProfileModal();
    }
  });

  // Handle ESC key
  const handleEscKey = (e) => {
    if (e.key === "Escape" && editProfileModalOpen) {
      closeEditProfileModal();
    }
  };

  document.addEventListener("keydown", handleEscKey);
  modalContainer.setAttribute("data-esc-handler", "true");

  showStatus("Membuka form edit profile...", "info");
}

function closeEditProfileModal() {
  const modalContainer = document.getElementById("editProfileModalContainer");
  if (modalContainer) {
    modalContainer.style.animation = "fadeOut 0.3s ease-out";

    setTimeout(() => {
      modalContainer.remove();
      document.body.style.overflow = "auto";
      editProfileModalOpen = false;
    }, 300);

    // Remove ESC key handler
    const escHandlers = document.querySelectorAll("[data-esc-handler]");
    escHandlers.forEach((handler) => {
      handler.removeAttribute("data-esc-handler");
    });
  }
}

// Update profile data function (called from edit profile modal)
function updateProfileData(profileData) {
  console.log("Profile updated:", profileData);

  // Update profile display in sidebar
  const profileName = document.querySelector(
    "#profileDropdownBtn .font-medium"
  );
  const profileEmail = document.querySelector("#profileDropdownBtn .text-sm");

  if (profileName && profileData.fullName) {
    profileName.textContent = profileData.fullName;
  }

  if (profileEmail && profileData.emailAddress) {
    profileEmail.textContent = profileData.emailAddress;
  }

  // Update profile photo if available
  const profileAvatar = document.querySelector(
    "#profileDropdownBtn .w-10.h-10"
  );
  if (profileAvatar && profileData.profilePhoto) {
    profileAvatar.innerHTML = `<img src="${profileData.profilePhoto}" alt="Profile" class="w-full h-full object-cover rounded-full">`;
  }

  showStatus("Profile berhasil diperbarui!", "success");
}

// Logout action
function handleLogout() {
  closeAllDropdowns();

  // Close mobile menu if open
  mobileMenu.classList.add("hidden");
  mobileOverlay.classList.add("hidden");
  mobileMenuIcon.classList.remove("hidden");
  mobileCloseIcon.classList.add("hidden");

  // Show logout confirmation
  const confirmLogout = confirm("Apakah Anda yakin ingin logout?");

  if (confirmLogout) {
    // Clear any stored user data
    localStorage.removeItem("userProfile");
    localStorage.removeItem("authToken");

    showStatus("Logout berhasil! Mengalihkan ke halaman login...", "success");

    // Redirect to login page
    setTimeout(() => {
      window.location.href = "login.html";
    }, 2000);
  }
}

// Responsive handling
window.addEventListener("resize", () => {
  if (window.innerWidth >= 768) {
    // Desktop view - close mobile menu
    mobileMenu.classList.add("hidden");
    mobileOverlay.classList.add("hidden");
    mobileMenuIcon.classList.remove("hidden");
    mobileCloseIcon.classList.add("hidden");
    closeAllDropdowns();
  }
});

// Make functions available globally for iframe communication
window.closeEditProfileModal = closeEditProfileModal;
window.updateProfileData = updateProfileData;
window.openEditProfileModal = openEditProfileModal;

// Initialize
document.addEventListener("DOMContentLoaded", () => {
  console.log("Navbar initialized successfully");
  showStatus("Navbar berhasil dimuat! Siap untuk digunakan.", "success");
});
