/**
 * Ultra-Modern Main TypeScript file for Lossless Bayesian Networks site
 * Copyright (C) 2025, Shyamal Chandra
 * Academic Research Site with Stunning Visual Effects
 */

// Lightweight Canvas Background Animation
class CanvasBackground {
    protected canvas: HTMLCanvasElement | null = null;
    protected ctx: CanvasRenderingContext2D | null = null;
    protected animationId: number | null = null;
    protected particles: Array<{x: number, y: number, vx: number, vy: number, size: number}> = [];
    private frameCount: number = 0;

    constructor() {
        const canvasElement = document.getElementById('backgroundCanvas') as HTMLCanvasElement;
        if (!canvasElement) return;
        
        this.canvas = canvasElement;
        this.ctx = this.canvas.getContext('2d');
        if (!this.ctx) return;

        this.init();
    }

    protected init(): void {
        this.resize();
        window.addEventListener('resize', () => this.resize());
        this.createParticles();
        this.animate();
    }

    protected resize(): void {
        if (!this.canvas) return;
        this.canvas.width = window.innerWidth;
        this.canvas.height = window.innerHeight;
        this.createParticles(); // Recreate particles on resize
    }

    protected createParticles(): void {
        if (!this.canvas) return;
        // Reduced particle count for better performance
        const particleCount = Math.min(20, Math.floor((window.innerWidth * window.innerHeight) / 50000));
        this.particles = [];
        
        for (let i = 0; i < particleCount; i++) {
            this.particles.push({
                x: Math.random() * this.canvas.width,
                y: Math.random() * this.canvas.height,
                vx: (Math.random() - 0.5) * 0.3,
                vy: (Math.random() - 0.5) * 0.3,
                size: Math.random() * 1.5 + 0.5
            });
        }
    }

    protected animate(): void {
        if (!this.ctx || !this.canvas) return;
        
        this.frameCount++;
        
        // Only clear and redraw every 2 frames for better performance
        if (this.frameCount % 2 === 0) {
            this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        }
        
        // Simplified drawing - no connections, just particles
        const ctx = this.ctx;
        const canvasWidth = this.canvas.width;
        const canvasHeight = this.canvas.height;
        ctx.fillStyle = 'rgba(37, 99, 235, 0.15)';
        
        this.particles.forEach(particle => {
            particle.x += particle.vx;
            particle.y += particle.vy;
            
            // Wrap around edges
            if (particle.x < 0) particle.x = canvasWidth;
            if (particle.x > canvasWidth) particle.x = 0;
            if (particle.y < 0) particle.y = canvasHeight;
            if (particle.y > canvasHeight) particle.y = 0;
            
            ctx.beginPath();
            ctx.arc(particle.x, particle.y, particle.size, 0, Math.PI * 2);
            ctx.fill();
        });
        
        this.animationId = requestAnimationFrame(() => this.animate());
    }

    public destroy(): void {
        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
        }
    }
}

// Lightweight Canvas Background - simplified version
class EnhancedCanvasBackground extends CanvasBackground {
    // Removed all heavy effects: network nodes, PDF icons, mouse tracking, waves
    // Just use the simple particle system from parent class
}

// PDF Modal Manager
class PDFModalManager {
    private modal: HTMLElement | null = null;
    private overlay: HTMLElement | null = null;
    private closeBtn: HTMLElement | null = null;
    private viewer: HTMLIFrameElement | null = null;
    private previewButtons: NodeListOf<HTMLElement> | null = null;

    constructor() {
        this.modal = document.getElementById('pdfModal');
        this.overlay = this.modal?.querySelector('.pdf-modal-overlay') as HTMLElement;
        this.closeBtn = document.getElementById('pdfModalClose');
        this.viewer = document.getElementById('pdfViewer') as HTMLIFrameElement;
        this.previewButtons = document.querySelectorAll('.btn-pdf-preview') as NodeListOf<HTMLElement>;

        this.init();
    }

    private init(): void {
        if (!this.modal || !this.closeBtn || !this.viewer) return;

        // Close button
        this.closeBtn.addEventListener('click', () => this.close());

        // Overlay click to close
        this.overlay?.addEventListener('click', () => this.close());

        // Escape key to close
        document.addEventListener('keydown', (e) => {
            if (e.key === 'Escape' && this.modal?.classList.contains('active')) {
                this.close();
            }
        });

        // Preview buttons
        this.previewButtons?.forEach(button => {
            button.addEventListener('click', (e) => {
                e.preventDefault();
                const pdfSrc = button.getAttribute('data-pdf-src');
                if (pdfSrc) {
                    this.open(pdfSrc);
                }
            });
        });

        // Add hover effects to PDF cards
        const pdfCards = document.querySelectorAll('.pdf-card');
        pdfCards.forEach(card => {
            card.addEventListener('mouseenter', () => {
                card.classList.add('pdf-card-hover');
            });
            card.addEventListener('mouseleave', () => {
                card.classList.remove('pdf-card-hover');
            });
        });
    }

    private open(pdfSrc: string): void {
        if (!this.modal || !this.viewer) return;

        // Set PDF source with proper path handling for GitHub Pages
        let finalSrc = pdfSrc;
        
        // Handle Jekyll relative_url filter output
        if (pdfSrc.includes('{{')) {
            // If it's a Jekyll template, it will be processed by Jekyll
            // But if we're in the browser, we need to handle it
            finalSrc = pdfSrc.replace(/{{.*?\|.*?}}/g, '').trim();
        }
        
        // Ensure path starts with / if it's a relative path
        if (!pdfSrc.startsWith('http') && !pdfSrc.startsWith('/')) {
            finalSrc = '/' + pdfSrc;
        }
        
        // For GitHub Pages with baseurl, ensure we have the correct path
        const baseurl = '/lossless-bayesian-networks';
        if (finalSrc.startsWith('/') && !finalSrc.startsWith(baseurl) && !finalSrc.startsWith('http')) {
            finalSrc = baseurl + finalSrc;
        }
        
        this.viewer.src = finalSrc + '#toolbar=1&navpanes=1&scrollbar=1';

        // Show modal
        this.modal.classList.add('active');
        document.body.style.overflow = 'hidden';

        // Add entrance animation
        setTimeout(() => {
            this.modal?.classList.add('modal-visible');
        }, 10);
    }

    private close(): void {
        if (!this.modal || !this.viewer) return;

        this.modal.classList.remove('active', 'modal-visible');
        document.body.style.overflow = '';

        // Clear iframe after animation
        setTimeout(() => {
            this.viewer!.src = '';
        }, 300);
    }
}

// Enhanced Navigation functionality
class Navigation {
    private navbar: HTMLElement | null;
    private navToggle: HTMLElement | null;
    private navMenu: HTMLElement | null;
    private navLinks: NodeListOf<HTMLElement>;
    private lastScrollY: number = 0;

    constructor() {
        this.navbar = document.getElementById('navbar');
        this.navToggle = document.getElementById('navToggle');
        this.navMenu = document.getElementById('navMenu');
        this.navLinks = document.querySelectorAll('.nav-link');
        this.lastScrollY = window.scrollY;
        
        this.init();
    }

    private init(): void {
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
                } else {
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
                        const targetElement = target as HTMLElement;
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
                } else {
                    this.navbar.classList.remove('scrolled');
                }
                
                // Hide/show navbar on scroll
                if (currentScrollY > this.lastScrollY && currentScrollY > 100) {
                    this.navbar.style.transform = 'translateY(-100%)';
                } else {
                    this.navbar.style.transform = 'translateY(0)';
                }
            }
            
            this.lastScrollY = currentScrollY;
        });

        // Active link highlighting with intersection observer
        this.updateActiveLink();
        window.addEventListener('scroll', () => this.updateActiveLink());
    }

    private updateActiveLink(): void {
        const sections = document.querySelectorAll('section[id]');
        const scrollY = window.pageYOffset + 150;

        sections.forEach(section => {
            const sectionElement = section as HTMLElement;
            if (!sectionElement) return;
            
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
                            this.createRippleEffect(button);
                        } catch (err) {
                            console.error('Failed to copy:', err);
                            this.showErrorFeedback(button);
                        }
                    }
                }
            });
        });
    }

    private showCopyFeedback(button: HTMLElement): void {
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

    private showErrorFeedback(button: HTMLElement): void {
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

    private createRippleEffect(button: HTMLElement): void {
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
    private observer: IntersectionObserver;
    private staggerDelay: number = 100;

    constructor() {
        this.observer = new IntersectionObserver(
            (entries) => {
                entries.forEach((entry, index) => {
                    if (entry.isIntersecting) {
                        setTimeout(() => {
                            entry.target.classList.add('animate');
                        }, index * this.staggerDelay);
                        this.observer.unobserve(entry.target);
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

// Removed ParallaxEffect - resource intensive

// Removed CursorEffects class - too resource intensive
// Using default browser cursor instead

// Lightweight Particle System - reduced particle count
class ParticleSystem {
    private container: HTMLElement | null;
    private particles: Particle[] = [];
    private particleCount: number = 15; // Reduced from 50
    private animationId: number | null = null;
    private frameCount: number = 0;

    constructor() {
        this.container = document.getElementById('particles');
        this.init();
    }

    private init(): void {
        if (!this.container) return;

        // Create fewer particles
        for (let i = 0; i < this.particleCount; i++) {
            const particle = new Particle(this.container);
            this.particles.push(particle);
        }

        // Start animation
        this.animate();
    }

    private animate(): void {
        this.frameCount++;
        // Update only every other frame for better performance
        if (this.frameCount % 2 === 0) {
            this.particles.forEach(particle => {
                particle.update();
            });
        }

        this.animationId = requestAnimationFrame(() => this.animate());
    }

    public destroy(): void {
        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
        }
        this.particles.forEach(particle => particle.destroy());
    }
}

class Particle {
    private element: HTMLElement;
    private x: number;
    private y: number;
    private vx: number;
    private vy: number;
    private size: number;
    private container: HTMLElement;

    constructor(container: HTMLElement) {
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

    public update(): void {
        this.x += this.vx;
        this.y += this.vy;

        // Wrap around edges
        if (this.x < 0) this.x = window.innerWidth;
        if (this.x > window.innerWidth) this.x = 0;
        if (this.y < 0) this.y = window.innerHeight;
        if (this.y > window.innerHeight) this.y = 0;

        this.element.style.left = `${this.x}px`;
        this.element.style.top = `${this.y}px`;
    }

    public destroy(): void {
        this.element.remove();
    }
}

// Enhanced Scroll Progress Indicator
class ScrollProgress {
    private progressBar: HTMLElement | null;

    constructor() {
        this.progressBar = document.getElementById('scrollProgress');
        this.init();
    }

    private init(): void {
        if (!this.progressBar) return;

        window.addEventListener('scroll', () => {
            const windowHeight = document.documentElement.scrollHeight - document.documentElement.clientHeight;
            const scrolled = (window.scrollY / windowHeight) * 100;
            this.progressBar!.style.width = `${scrolled}%`;
        });
    }
}

// Removed MagneticButtons - resource intensive

// Stagger Animation Manager
class StaggerAnimation {
    private observer: IntersectionObserver;

    constructor() {
        this.observer = new IntersectionObserver(
            (entries) => {
                entries.forEach((entry, index) => {
                    if (entry.isIntersecting) {
                        setTimeout(() => {
                            entry.target.classList.add('animate');
                        }, index * 100);
                        this.observer.unobserve(entry.target);
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
    new EnhancedCanvasBackground();
    new Navigation();
    new TabManager();
    new ClipboardManager();
    new AnimationObserver();
    // Removed ParallaxEffect - resource intensive
    // Removed CursorEffects - resource intensive
    new ParticleSystem();
    new ScrollProgress();
    // Removed MagneticButtons - resource intensive
    new StaggerAnimation();
    new PDFModalManager();
    
    // Add smooth page load animation
    document.body.style.opacity = '0';
    setTimeout(() => {
        document.body.style.transition = 'opacity 0.5s ease';
        document.body.style.opacity = '1';
    }, 100);
    
    // Add entrance animations to hero elements
    const heroElements = document.querySelectorAll('.hero-badge, .hero-title, .hero-description, .hero-actions, .hero-stats');
    heroElements.forEach((el, index) => {
        (el as HTMLElement).style.opacity = '0';
        (el as HTMLElement).style.transform = 'translateY(30px)';
        setTimeout(() => {
            (el as HTMLElement).style.transition = 'all 0.8s cubic-bezier(0.4, 0, 0.2, 1)';
            (el as HTMLElement).style.opacity = '1';
            (el as HTMLElement).style.transform = 'translateY(0)';
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
        anchor.addEventListener('click', (e: Event) => {
            const href = (anchor as HTMLAnchorElement).getAttribute('href');
            if (href && href !== '#') {
                e.preventDefault();
                const target = document.querySelector(href);
                if (target) {
                    const offset = 80;
                    const targetPosition = (target as HTMLElement).offsetTop - offset;
                    window.scrollTo({
                        top: targetPosition,
                        behavior: 'smooth'
                    });
                }
            }
        });
    });
    
    // Removed parallax effect - resource intensive
    
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
