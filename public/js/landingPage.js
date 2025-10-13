// Hamburger menu toggle
const menuBtn = document.getElementById("menu-btn");
const mobileMenu = document.getElementById("mobile-menu");
if (menuBtn && mobileMenu) {
  menuBtn.addEventListener("click", () => {
    mobileMenu.classList.toggle("hidden");
  });
}

// Simple AOS-like animation
function animateOnScroll() {
  const elements = document.querySelectorAll("[data-aos]");
  const windowHeight = window.innerHeight;
  elements.forEach((el) => {
    const rect = el.getBoundingClientRect();
    if (rect.top < windowHeight - 50) {
      const type = el.getAttribute("data-aos");
      const duration = el.getAttribute("data-aos-duration") || 800;
      const delay = el.getAttribute("data-aos-delay") || 0;
      el.style.transition = `all ${duration}ms cubic-bezier(.4,0,.2,1) ${delay}ms`;
      if (type === "fade-up") {
        el.style.opacity = 1;
        el.style.transform = "translateY(0)";
      } else if (type === "fade-down") {
        el.style.opacity = 1;
        el.style.transform = "translateY(0)";
      } else if (type === "fade-right") {
        el.style.opacity = 1;
        el.style.transform = "translateX(0)";
      } else if (type === "fade-left") {
        el.style.opacity = 1;
        el.style.transform = "translateX(0)";
      } else if (type === "zoom-in") {
        el.style.opacity = 1;
        el.style.transform = "scale(1)";
      }
    }
  });
}

function setInitialAOS() {
  const elements = document.querySelectorAll("[data-aos]");
  elements.forEach((el) => {
    const type = el.getAttribute("data-aos");
    el.style.opacity = 0;
    if (type === "fade-up") {
      el.style.transform = "translateY(40px)";
    } else if (type === "fade-down") {
      el.style.transform = "translateY(-40px)";
    } else if (type === "fade-right") {
      el.style.transform = "translateX(-40px)";
    } else if (type === "fade-left") {
      el.style.transform = "translateX(40px)";
    } else if (type === "zoom-in") {
      el.style.transform = "scale(0.8)";
    }
  });
}

document.addEventListener("DOMContentLoaded", () => {
  setInitialAOS();
  animateOnScroll();
  window.addEventListener("scroll", animateOnScroll);
});
