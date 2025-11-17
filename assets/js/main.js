"use strict";
/**
 * Enhanced Main TypeScript file for Lossless Bayesian Networks site
 * Copyright (C) 2025, Shyamal Chandra
 * Jazzy and Snazzy UX with Awesome Interactions
 */
// Enhanced Navigation functionality
class Navigation {
    constructor() {
        this.lastScrollY = 0;
        this.navbar = document.getElementById('navbar');
        this.navToggle = document.getElementById('navToggle');
        this.navMenu = document.getElementById('navMenu');
        this.navLinks = document.querySelectorAll('.nav-link');
        this.lastScrollY = window.scrollY;
        this.init();
    }
    init() {
        // Mobile menu toggle
        if (this.navToggle && this.navMenu) {
            const navMenu = this.navMenu;
            const navToggle = this.navToggle;
            this.navToggle.addEventListener('click', () => {
                navMenu.classList.toggle('active');
                // Animate hamburger icon
                const spans = navToggle.querySelectorAll('span');
                if (navMenu.classList.contains('active')) {
                    spans[0].style.transform = 'rotate(45deg) translate(5px, 5px)';
                    spans[1].style.opacity = '0';
                    spans[2].style.transform = 'rotate(-45deg) translate(7px, -6px)';
                }
                else {
                    spans[0].style.transform = '';
                    spans[1].style.opacity = '1';
                    spans[2].style.transform = '';
                }
            });
        }
        // Smooth scrolling for anchor links
        this.navLinks.forEach(link => {
            link.addEventListener('click', (e) => {
                const href = link.getAttribute('href');
                if (href && href.startsWith('#')) {
                    e.preventDefault();
                    const target = document.querySelector(href);
                    if (target) {
                        const offset = 80;
                        const targetElement = target;
                        if (targetElement) {
                            const targetPosition = targetElement.offsetTop - offset;
                            window.scrollTo({
                                top: targetPosition,
                                behavior: 'smooth'
                            });
                        }
                        this.navMenu?.classList.remove('active');
                    }
                }
            });
        });
        // Enhanced navbar scroll effect with direction detection
        window.addEventListener('scroll', () => {
            const currentScrollY = window.scrollY;
            if (this.navbar) {
                if (currentScrollY > 50) {
                    this.navbar.classList.add('scrolled');
                }
                else {
                    this.navbar.classList.remove('scrolled');
                }
                // Hide/show navbar on scroll
                if (currentScrollY > this.lastScrollY && currentScrollY > 100) {
                    this.navbar.style.transform = 'translateY(-100%)';
                }
                else {
                    this.navbar.style.transform = 'translateY(0)';
                }
            }
            this.lastScrollY = currentScrollY;
        });
        // Active link highlighting with intersection observer
        this.updateActiveLink();
        window.addEventListener('scroll', () => this.updateActiveLink());
    }
    updateActiveLink() {
        const sections = document.querySelectorAll('section[id]');
        const scrollY = window.pageYOffset + 150;
        sections.forEach(section => {
            const sectionElement = section;
            if (!sectionElement)
                return;
            const sectionHeight = sectionElement.clientHeight;
            const sectionTop = sectionElement.offsetTop;
            const sectionId = section.getAttribute('id');
            if (scrollY > sectionTop && scrollY <= sectionTop + sectionHeight) {
                this.navLinks.forEach(link => {
                    link.classList.remove('active');
                    if (link.getAttribute('href') === `#${sectionId}`) {
                        link.classList.add('active');
                    }
                });
            }
        });
    }
}
// Enhanced Tab functionality
class TabManager {
    constructor() {
        this.tabButtons = document.querySelectorAll('.tab-btn');
        this.tabContents = document.querySelectorAll('.tab-content');
        this.init();
    }
    init() {
        this.tabButtons.forEach(button => {
            button.addEventListener('click', () => {
                const tabId = button.getAttribute('data-tab');
                // Remove active class from all buttons and contents
                this.tabButtons.forEach(btn => {
                    btn.classList.remove('active');
                    btn.style.transform = 'scale(1)';
                });
                this.tabContents.forEach(content => {
                    content.style.opacity = '0';
                    content.style.transform = 'translateY(10px)';
                    setTimeout(() => {
                        content.classList.remove('active');
                    }, 200);
                });
                // Add active class to clicked button with animation
                button.classList.add('active');
                button.style.transform = 'scale(1.05)';
                // Show corresponding content with fade-in
                if (tabId) {
                    const content = document.getElementById(`${tabId}-tab`);
                    if (content) {
                        setTimeout(() => {
                            content.classList.add('active');
                            content.style.opacity = '1';
                            content.style.transform = 'translateY(0)';
                        }, 200);
                    }
                }
            });
        });
    }
}
// Enhanced Copy to clipboard functionality
class ClipboardManager {
    constructor() {
        this.copyButtons = document.querySelectorAll('.copy-btn');
        this.init();
    }
    init() {
        this.copyButtons.forEach(button => {
            button.addEventListener('click', async () => {
                const targetId = button.getAttribute('data-copy');
                if (targetId) {
                    const target = document.getElementById(targetId);
                    if (target) {
                        const text = target.textContent || '';
                        try {
                            await navigator.clipboard.writeText(text);
                            this.showCopyFeedback(button);
                            this.createRippleEffect(button);
                        }
                        catch (err) {
                            console.error('Failed to copy:', err);
                            this.showErrorFeedback(button);
                        }
                    }
                }
            });
        });
    }
    showCopyFeedback(button) {
        const originalText = button.textContent;
        button.textContent = '✓ Copied!';
        button.style.background = 'rgba(34, 197, 94, 0.2)';
        button.style.borderColor = 'rgba(34, 197, 94, 0.3)';
        button.style.color = '#22c55e';
        button.style.transform = 'scale(1.1)';
        setTimeout(() => {
            button.textContent = originalText;
            button.style.background = '';
            button.style.borderColor = '';
            button.style.color = '';
            button.style.transform = '';
        }, 2000);
    }
    showErrorFeedback(button) {
        const originalText = button.textContent;
        button.textContent = '✗ Error';
        button.style.background = 'rgba(239, 68, 68, 0.2)';
        button.style.borderColor = 'rgba(239, 68, 68, 0.3)';
        button.style.color = '#ef4444';
        setTimeout(() => {
            button.textContent = originalText;
            button.style.background = '';
            button.style.borderColor = '';
            button.style.color = '';
        }, 2000);
    }
    createRippleEffect(button) {
        const ripple = document.createElement('span');
        ripple.style.position = 'absolute';
        ripple.style.borderRadius = '50%';
        ripple.style.background = 'rgba(34, 197, 94, 0.6)';
        ripple.style.width = '20px';
        ripple.style.height = '20px';
        ripple.style.left = '50%';
        ripple.style.top = '50%';
        ripple.style.transform = 'translate(-50%, -50%) scale(0)';
        ripple.style.animation = 'ripple 0.6s ease-out';
        ripple.style.pointerEvents = 'none';
        button.style.position = 'relative';
        button.style.overflow = 'hidden';
        button.appendChild(ripple);
        setTimeout(() => {
            ripple.remove();
        }, 600);
    }
}
// Enhanced Intersection Observer for animations
class AnimationObserver {
    constructor() {
        this.staggerDelay = 100;
        this.observer = new IntersectionObserver((entries) => {
            entries.forEach((entry, index) => {
                if (entry.isIntersecting) {
                    setTimeout(() => {
                        entry.target.classList.add('animate');
                    }, index * this.staggerDelay);
                    this.observer.unobserve(entry.target);
                }
            });
        }, {
            threshold: 0.1,
            rootMargin: '0px 0px -50px 0px'
        });
        this.init();
    }
    init() {
        const elements = document.querySelectorAll('[data-aos]');
        elements.forEach(el => {
            this.observer.observe(el);
        });
    }
}
// Parallax effect for hero section
class ParallaxEffect {
    constructor() {
        this.hero = document.querySelector('.hero');
        this.init();
    }
    init() {
        if (!this.hero)
            return;
        window.addEventListener('scroll', () => {
            const scrolled = window.pageYOffset;
            const rate = scrolled * 0.5;
            const heroPattern = this.hero?.querySelector('.hero-pattern');
            if (heroPattern) {
                heroPattern.style.transform = `translateY(${rate}px)`;
            }
        });
    }
}
// Mouse cursor effects
class CursorEffects {
    constructor() {
        this.cursor = null;
        this.cursorFollower = null;
        this.createCursor();
        this.init();
    }
    createCursor() {
        this.cursor = document.createElement('div');
        this.cursor.className = 'custom-cursor';
        this.cursor.style.cssText = `
            width: 10px;
            height: 10px;
            border: 2px solid var(--primary-color);
            border-radius: 50%;
            position: fixed;
            pointer-events: none;
            z-index: 9999;
            transition: transform 0.1s ease;
            display: none;
        `;
        this.cursorFollower = document.createElement('div');
        this.cursorFollower.className = 'cursor-follower';
        this.cursorFollower.style.cssText = `
            width: 30px;
            height: 30px;
            border: 1px solid rgba(99, 102, 241, 0.3);
            border-radius: 50%;
            position: fixed;
            pointer-events: none;
            z-index: 9998;
            transition: all 0.3s ease;
            display: none;
        `;
        document.body.appendChild(this.cursor);
        document.body.appendChild(this.cursorFollower);
    }
    init() {
        // Only enable on desktop
        if (window.matchMedia('(pointer: fine)').matches) {
            this.cursor.style.display = 'block';
            this.cursorFollower.style.display = 'block';
            document.addEventListener('mousemove', (e) => {
                if (this.cursor && this.cursorFollower) {
                    this.cursor.style.left = `${e.clientX}px`;
                    this.cursor.style.top = `${e.clientY}px`;
                    setTimeout(() => {
                        this.cursorFollower.style.left = `${e.clientX - 15}px`;
                        this.cursorFollower.style.top = `${e.clientY - 15}px`;
                    }, 100);
                }
            });
            // Hover effects
            const interactiveElements = document.querySelectorAll('a, button, .btn, .feature-card, .doc-card');
            interactiveElements.forEach(el => {
                el.addEventListener('mouseenter', () => {
                    if (this.cursor && this.cursorFollower) {
                        this.cursor.style.transform = 'scale(1.5)';
                        this.cursorFollower.style.transform = 'scale(1.5)';
                        this.cursorFollower.style.borderColor = 'rgba(99, 102, 241, 0.6)';
                    }
                });
                el.addEventListener('mouseleave', () => {
                    if (this.cursor && this.cursorFollower) {
                        this.cursor.style.transform = 'scale(1)';
                        this.cursorFollower.style.transform = 'scale(1)';
                        this.cursorFollower.style.borderColor = 'rgba(99, 102, 241, 0.3)';
                    }
                });
            });
        }
    }
}
// Particle System for Background
class ParticleSystem {
    constructor() {
        this.particles = [];
        this.particleCount = 50;
        this.animationId = null;
        this.container = document.getElementById('particles');
        this.init();
    }
    init() {
        if (!this.container)
            return;
        // Create particles
        for (let i = 0; i < this.particleCount; i++) {
            const particle = new Particle(this.container);
            this.particles.push(particle);
        }
        // Start animation
        this.animate();
    }
    animate() {
        this.particles.forEach(particle => {
            particle.update();
        });
        this.animationId = requestAnimationFrame(() => this.animate());
    }
    destroy() {
        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
        }
        this.particles.forEach(particle => particle.destroy());
    }
}
class Particle {
    constructor(container) {
        this.container = container;
        this.element = document.createElement('div');
        this.element.className = 'particle';
        // Random starting position
        this.x = Math.random() * window.innerWidth;
        this.y = Math.random() * window.innerHeight;
        // Random velocity
        this.vx = (Math.random() - 0.5) * 0.5;
        this.vy = (Math.random() - 0.5) * 0.5;
        // Random size
        this.size = Math.random() * 3 + 1;
        // Apply styles
        this.element.style.width = `${this.size}px`;
        this.element.style.height = `${this.size}px`;
        this.element.style.left = `${this.x}px`;
        this.element.style.top = `${this.y}px`;
        // Random color from gradient
        const colors = ['rgba(99, 102, 241, 0.4)', 'rgba(139, 92, 246, 0.4)', 'rgba(236, 72, 153, 0.4)'];
        this.element.style.background = colors[Math.floor(Math.random() * colors.length)];
        container.appendChild(this.element);
    }
    update() {
        this.x += this.vx;
        this.y += this.vy;
        // Wrap around edges
        if (this.x < 0)
            this.x = window.innerWidth;
        if (this.x > window.innerWidth)
            this.x = 0;
        if (this.y < 0)
            this.y = window.innerHeight;
        if (this.y > window.innerHeight)
            this.y = 0;
        this.element.style.left = `${this.x}px`;
        this.element.style.top = `${this.y}px`;
    }
    destroy() {
        this.element.remove();
    }
}
// Enhanced Scroll Progress Indicator
class ScrollProgress {
    constructor() {
        this.progressBar = document.getElementById('scrollProgress');
        this.init();
    }
    init() {
        if (!this.progressBar)
            return;
        window.addEventListener('scroll', () => {
            const windowHeight = document.documentElement.scrollHeight - document.documentElement.clientHeight;
            const scrolled = (window.scrollY / windowHeight) * 100;
            this.progressBar.style.width = `${scrolled}%`;
        });
    }
}
// Magnetic Button Effect
class MagneticButtons {
    constructor() {
        this.buttons = document.querySelectorAll('.magnetic-btn');
        this.init();
    }
    init() {
        this.buttons.forEach(button => {
            button.addEventListener('mousemove', (e) => {
                const rect = button.getBoundingClientRect();
                const x = e.clientX - rect.left - rect.width / 2;
                const y = e.clientY - rect.top - rect.height / 2;
                const moveX = x * 0.3;
                const moveY = y * 0.3;
                button.style.transform = `translate(${moveX}px, ${moveY}px) scale(1.05)`;
            });
            button.addEventListener('mouseleave', () => {
                button.style.transform = 'translate(0, 0) scale(1)';
            });
        });
    }
}
// Stagger Animation Manager
class StaggerAnimation {
    constructor() {
        this.observer = new IntersectionObserver((entries) => {
            entries.forEach((entry, index) => {
                if (entry.isIntersecting) {
                    setTimeout(() => {
                        entry.target.classList.add('animate');
                    }, index * 100);
                    this.observer.unobserve(entry.target);
                }
            });
        }, {
            threshold: 0.1,
            rootMargin: '0px 0px -50px 0px'
        });
        this.init();
    }
    init() {
        const staggerContainers = document.querySelectorAll('.fade-in-up-stagger');
        staggerContainers.forEach(container => {
            const children = container.children;
            Array.from(children).forEach(child => {
                this.observer.observe(child);
            });
        });
    }
}
// Add CSS for ripple animation and particles
const style = document.createElement('style');
style.textContent = `
    @keyframes ripple {
        to {
            transform: translate(-50%, -50%) scale(4);
            opacity: 0;
        }
    }
    
    @keyframes floatParticle {
        0%, 100% {
            transform: translateY(0) translateX(0);
        }
        50% {
            transform: translateY(-20px) translateX(10px);
        }
    }
    
    .particle {
        position: absolute;
        border-radius: 50%;
        pointer-events: none;
        transition: transform 0.1s ease-out;
    }
`;
document.head.appendChild(style);
// Initialize everything when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    new Navigation();
    new TabManager();
    new ClipboardManager();
    new AnimationObserver();
    new ParallaxEffect();
    new CursorEffects();
    new ParticleSystem();
    new ScrollProgress();
    new MagneticButtons();
    new StaggerAnimation();
    // Add smooth page load animation
    document.body.style.opacity = '0';
    setTimeout(() => {
        document.body.style.transition = 'opacity 0.5s ease';
        document.body.style.opacity = '1';
    }, 100);
    // Add entrance animations to hero elements
    const heroElements = document.querySelectorAll('.hero-badge, .hero-title, .hero-description, .hero-actions, .hero-stats');
    heroElements.forEach((el, index) => {
        el.style.opacity = '0';
        el.style.transform = 'translateY(30px)';
        setTimeout(() => {
            el.style.transition = 'all 0.8s cubic-bezier(0.4, 0, 0.2, 1)';
            el.style.opacity = '1';
            el.style.transform = 'translateY(0)';
        }, index * 150 + 200);
    });
    // Add scroll-triggered reveal animations
    const revealObserver = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.classList.add('reveal-on-scroll');
                revealObserver.unobserve(entry.target);
            }
        });
    }, { threshold: 0.1 });
    document.querySelectorAll('section, .feature-card, .doc-card, .example-card').forEach(el => {
        revealObserver.observe(el);
    });
    // Add interactive hover effects to cards
    document.querySelectorAll('.feature-card, .doc-card, .example-card').forEach(card => {
        card.addEventListener('mouseenter', () => {
            card.classList.add('interactive');
        });
    });
    // Add smooth scroll to all anchor links
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', (e) => {
            const href = anchor.getAttribute('href');
            if (href && href !== '#') {
                e.preventDefault();
                const target = document.querySelector(href);
                if (target) {
                    const offset = 80;
                    const targetPosition = target.offsetTop - offset;
                    window.scrollTo({
                        top: targetPosition,
                        behavior: 'smooth'
                    });
                }
            }
        });
    });
    // Add parallax effect to hero background on scroll
    let lastScroll = 0;
    window.addEventListener('scroll', () => {
        const currentScroll = window.pageYOffset;
        const hero = document.querySelector('.hero-background');
        if (hero) {
            const rate = currentScroll * 0.3;
            hero.style.transform = `translateY(${rate}px)`;
        }
        lastScroll = currentScroll;
    });
    // Add loading class for smooth page load
    document.body.classList.add('loading');
    setTimeout(() => {
        document.body.classList.remove('loading');
    }, 600);
});
// Smooth scroll polyfill for older browsers
if (!('scrollBehavior' in document.documentElement.style)) {
    const script = document.createElement('script');
    script.src = 'https://cdn.jsdelivr.net/gh/cferdinandi/smooth-scroll@15/dist/smooth-scroll.polyfills.min.js';
    document.head.appendChild(script);
}
//# sourceMappingURL=main.js.map