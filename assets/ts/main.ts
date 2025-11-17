/**
 * Main TypeScript file for Lossless Bayesian Networks site
 * Copyright (C) 2025, Shyamal Chandra
 */

// Navigation functionality
class Navigation {
    private navbar: HTMLElement | null;
    private navToggle: HTMLElement | null;
    private navMenu: HTMLElement | null;
    private navLinks: NodeListOf<HTMLElement>;

    constructor() {
        this.navbar = document.getElementById('navbar');
        this.navToggle = document.getElementById('navToggle');
        this.navMenu = document.getElementById('navMenu');
        this.navLinks = document.querySelectorAll('.nav-link');
        
        this.init();
    }

    private init(): void {
        // Mobile menu toggle
        if (this.navToggle && this.navMenu) {
            this.navToggle.addEventListener('click', () => {
                this.navMenu.classList.toggle('active');
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
                        target.scrollIntoView({ behavior: 'smooth' });
                        if (this.navMenu) {
                            this.navMenu.classList.remove('active');
                        }
                    }
                }
            });
        });

        // Navbar scroll effect
        window.addEventListener('scroll', () => {
            if (this.navbar) {
                if (window.scrollY > 50) {
                    this.navbar.classList.add('scrolled');
                } else {
                    this.navbar.classList.remove('scrolled');
                }
            }
        });

        // Active link highlighting
        this.updateActiveLink();
        window.addEventListener('scroll', () => this.updateActiveLink());
    }

    private updateActiveLink(): void {
        const sections = document.querySelectorAll('section[id]');
        const scrollY = window.pageYOffset;

        sections.forEach(section => {
            const sectionElement = section as HTMLElement;
            if (!sectionElement) return;
            
            const sectionHeight = sectionElement.clientHeight;
            const sectionTop = sectionElement.offsetTop - 100;
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

// Tab functionality
class TabManager {
    private tabButtons: NodeListOf<HTMLElement>;
    private tabContents: NodeListOf<HTMLElement>;

    constructor() {
        this.tabButtons = document.querySelectorAll('.tab-btn');
        this.tabContents = document.querySelectorAll('.tab-content');
        this.init();
    }

    private init(): void {
        this.tabButtons.forEach(button => {
            button.addEventListener('click', () => {
                const tabId = button.getAttribute('data-tab');
                
                // Remove active class from all buttons and contents
                this.tabButtons.forEach(btn => btn.classList.remove('active'));
                this.tabContents.forEach(content => content.classList.remove('active'));
                
                // Add active class to clicked button
                button.classList.add('active');
                
                // Show corresponding content
                if (tabId) {
                    const content = document.getElementById(`${tabId}-tab`);
                    if (content) {
                        content.classList.add('active');
                    }
                }
            });
        });
    }
}

// Copy to clipboard functionality
class ClipboardManager {
    private copyButtons: NodeListOf<HTMLElement>;

    constructor() {
        this.copyButtons = document.querySelectorAll('.copy-btn');
        this.init();
    }

    private init(): void {
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
                        } catch (err) {
                            console.error('Failed to copy:', err);
                        }
                    }
                }
            });
        });
    }

    private showCopyFeedback(button: HTMLElement): void {
        const originalText = button.textContent;
        button.textContent = 'Copied!';
        button.style.background = 'rgba(34, 197, 94, 0.2)';
        button.style.borderColor = 'rgba(34, 197, 94, 0.3)';
        button.style.color = '#22c55e';
        
        setTimeout(() => {
            button.textContent = originalText;
            button.style.background = '';
            button.style.borderColor = '';
            button.style.color = '';
        }, 2000);
    }
}

// Intersection Observer for animations
class AnimationObserver {
    private observer: IntersectionObserver;

    constructor() {
        this.observer = new IntersectionObserver(
            (entries) => {
                entries.forEach(entry => {
                    if (entry.isIntersecting) {
                        entry.target.classList.add('animate');
                    }
                });
            },
            {
                threshold: 0.1,
                rootMargin: '0px 0px -50px 0px'
            }
        );
        this.init();
    }

    private init(): void {
        const elements = document.querySelectorAll('[data-aos]');
        elements.forEach(el => {
            this.observer.observe(el);
        });
    }
}

// Initialize everything when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    new Navigation();
    new TabManager();
    new ClipboardManager();
    new AnimationObserver();
});

// Smooth scroll polyfill for older browsers
if (!('scrollBehavior' in document.documentElement.style)) {
    const script = document.createElement('script');
    script.src = 'https://cdn.jsdelivr.net/gh/cferdinandi/smooth-scroll@15/dist/smooth-scroll.polyfills.min.js';
    document.head.appendChild(script);
}
