
//toggles navigation on mobile phone

function toggleNav() {
    document.querySelector(".burger").addEventListener("click", () => {
        document.querySelector(".navlist").classList.toggle("nav-toggle");
    });  
}

toggleNav();