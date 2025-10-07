// Slider logic
document.addEventListener("DOMContentLoaded", function () {
  const track = document.getElementById("slider-track");
  const prevBtn = document.getElementById("prev-btn");
  const nextBtn = document.getElementById("next-btn");
  let slides = track ? Array.from(track.children) : [];
  let intervalId;
  let current = 1; // Start at 1 because of prepended clone
  // Infinite carousel: clone last to front, first to end
  if (track && slides.length > 0 && !track.dataset.cloned) {
    const firstClone = slides[0].cloneNode(true);
    const lastClone = slides[slides.length - 1].cloneNode(true);
    // Ensure clones have the same classes
    firstClone.className = slides[0].className;
    lastClone.className = slides[slides.length - 1].className;
    track.insertBefore(lastClone, slides[0]);
    track.appendChild(firstClone);
    track.dataset.cloned = "true";
    slides = Array.from(track.children); // update after clone
  }
  let isTransitioning = false;
  function updateSlider(animate = true, force = false) {
    if (track) {
      // If animating and force is true, snap immediately
      if (isTransitioning && force) {
        track.style.transition = "none";
        track.style.transform = `translateX(-${current * 100}%)`;
        // Force reflow
        void track.offsetWidth;
        isTransitioning = false;
      }
      track.style.transition = animate ? "transform 0.5s" : "none";
      track.style.transform = `translateX(-${current * 100}%)`;
      isTransitioning = animate;
    }
  }
  function handleTransitionEnd() {
    isTransitioning = false;
    if (current === slides.length - 1) {
      // If at (clone of first), snap to real first
      track.style.transition = "none";
      current = 1;
      updateSlider(false);
    } else if (current === 0) {
      // If at (clone of last), snap to real last
      track.style.transition = "none";
      current = slides.length - 2;
      updateSlider(false);
    }
  }
  if (track) {
    track.addEventListener("transitionend", handleTransitionEnd);
  }
  function startAutoSlide() {
    intervalId = setInterval(() => {
      current++;
      updateSlider(true);
    }, 4000);
  }
  function stopAutoSlide() {
    clearInterval(intervalId);
  }
  let canScroll = true;
  let lastScrollTime = Date.now();
  function allowScrollAfterDelay() {
    canScroll = false;
    setTimeout(() => {
      canScroll = true;
    }, 1000); // 1 detik
  }
  if (prevBtn && nextBtn && slides.length > 0) {
    prevBtn.addEventListener("click", function () {
      if (!canScroll) return;
      updateSlider(false, true);
      current--;
      updateSlider(true);
      stopAutoSlide();
      startAutoSlide();
      allowScrollAfterDelay();
    });
    nextBtn.addEventListener("click", function () {
      if (!canScroll) return;
      updateSlider(false, true);
      current++;
      updateSlider(true);
      stopAutoSlide();
      startAutoSlide();
      allowScrollAfterDelay();
    });
    // Set initial position to first real slide
    updateSlider(false);
    startAutoSlide();
    allowScrollAfterDelay();
  }
});