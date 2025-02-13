import React, { useState, useEffect } from "react";
import {
  Moon,
  Sun,
  Github,
  Terminal,
  Download,
  ChevronRight,
} from "lucide-react";
import "../home.css";
// Typing effect hook
const useTypingEffect = (text, speed = 50) => {
  const [displayText, setDisplayText] = useState("");

  useEffect(() => {
    let i = 0;
    const timer = setInterval(() => {
      if (i < text.length) {
        setDisplayText((prev) => prev + text.charAt(i));
        i++;
      } else {
        clearInterval(timer);
      }
    }, speed);

    return () => clearInterval(timer);
  }, [text, speed]);

  return displayText;
};

const TerminalWindow = () => {
  return (
    <div className="terminal-window">
      <div className="terminal-buttons">
        <div className="terminal-button red"></div>
        <div className="terminal-button yellow"></div>
        <div className="terminal-button green"></div>
      </div>
      <pre className="terminal-content">
        <span className="terminal-prompt">$ </span>
        <span className="terminal-command">ctodo --new-task</span>
        <br />
        <span className="terminal-output">&gt;Creating new task...</span>
        <br />
        <span className="terminal-output">&gt;Task added successfully!</span>
        <br />
        <span className="terminal-prompt">$ </span>
        <span className="terminal-command">ctodo --list</span>
        <br />
        <span className="terminal-output">&gt;Displaying all tasks...</span>
        <br />
        <span className="terminal-check">✓ </span>
        <span className="terminal-list-item">Complete C Project.</span>
        <br />
        <span className="terminal-list-item">- Complete Assignment. </span>
        <br />
        <span className="terminal-list-item">- Write documentation</span>
      </pre>
    </div>
  );
};

const Home = () => {
  const [isDarkMode, setIsDarkMode] = useState(true);

  const toggleTheme = () => {
    setIsDarkMode(!isDarkMode);
  };

  return (
    <div className={`app ${isDarkMode ? "dark-mode" : "light-mode"}`}>
      {/* Navigation */}
      <nav className="navbar">
        <div className="navbar-container">
          <div className="logo">
            <Terminal className="logo-icon" />
            <span className="logo-text">C TODO</span>
          </div>
          <div className="nav-buttons">
            <button onClick={toggleTheme} className="theme-toggle">
              {isDarkMode ? (
                <Sun className="icon" />
              ) : (
                <Moon className="icon" />
              )}
            </button>
            <a
              target="_blank"
              href="https://github.com/rajeshkhadka200/ctodo"
              className="github-link"
            >
              <Github className="icon" />
              <span>GitHub</span>
            </a>
          </div>
        </div>
      </nav>

      {/* Hero Section */}
      <section className="hero">
        <div className="hero-container">
          <h1 className="hero-title">
            Power Up Your Terminal
            <br />
            <span className="highlight">One Task at a Time</span>
          </h1>
          <p className="hero-subtitle">
            Light and fast task management app, built with pure C.
          </p>
          <div className="download-button-container">
            <a href="/todo.exe" download={true}>
              <button className="download-button">
                <Download className="icon" />
                Download For Windows
              </button>
            </a>
          </div>
          <div className="terminal-container">
            <TerminalWindow />
          </div>
        </div>
      </section>

      {/* Getting Started */}
      <section className="getting-started">
        <div className="section-container">
          <h2 className="section-title">Getting Started</h2>
          <div className="steps-container">
            {[
              {
                step: 1,
                command: "$ download todo.exe",
                desc: "Download the executable",
              },
              {
                step: 2,
                command: "$ cd todo.exe",
                desc: "Initialize todo app.",
              },
              {
                step: 3,
                command: '$ ./todo add "My first task"',
                desc: "Add your first task",
              },
              { step: 4, command: "$ ./todo list", desc: "View all tasks" },
            ].map(({ step, command, desc }) => (
              <div key={step} className="step-item">
                <span className="step-number">{step}</span>
                <code className="step-command">{command}</code>
                <ChevronRight className="step-arrow" />
                <span className="step-description">{desc}</span>
              </div>
            ))}
          </div>
        </div>
      </section>

      {/* Features */}
      <section className="features">
        <div className="section-container">
          <h2 className="section-title">Why Choose ToDo CLI?</h2>
          <div className="features-grid">
            {[
              {
                title: "Offline First",
                desc: "Work anywhere, anytime. No internet required.",
              },
              {
                title: "Resource Efficient",
                desc: "Minimal CPU and memory footprint.",
              },
              {
                title: "Data Portable",
                desc: "Simple file-based storage you can backup.",
              },

              {
                title: "Zero Dependencies",
                desc: "Just one executable, nothing else needed.",
              },
            ].map(({ title, desc }) => (
              <div key={title} className="feature-card">
                <h3 className="feature-title">{title}</h3>
                <p className="feature-description">{desc}</p>
              </div>
            ))}
          </div>
        </div>
      </section>

      {/* Footer */}
      <footer className="footer">
        <div className="footer-container">
          <a
            target="_blank"
            href="https://github.com/rajeshkhadka200/ctodo"
            className="github-link"
          >
            <Github className="icon" />
            <span>View on GitHub</span>
          </a>
          <p className="footer-text">
            Built with <span className="heart">❤️</span> by Team RSSB
          </p>
        </div>
      </footer>
    </div>
  );
};

export default Home;
