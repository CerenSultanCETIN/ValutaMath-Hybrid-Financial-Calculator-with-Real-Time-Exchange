📊 ValutaMath: Hybrid Financial Calculator & Real-Time Exchange Engine

<img width="229" height="422" alt="image" src="https://github.com/user-attachments/assets/514e8d8a-9f96-4437-9150-0ddcfb598311" />
<img width="227" height="423" alt="image" src="https://github.com/user-attachments/assets/a77e2404-98f5-40c0-93f0-e587c96c734a" />



ValutaMath is a high-performance, cross-platform hybrid application developed with C++17 and Qt 6 Framework. It merges a sophisticated mathematical calculation engine with an asynchronous financial data retrieval system. Designed with an Apple-inspired minimalist aesthetic, it offers a seamless user experience for both complex arithmetic and real-time currency conversion.

🚀 Key Features / Öne Çıkan Özellikler
Hybrid Calculation Engine: Execute complex arithmetic operations with parenthesis support and mathematical operator precedence (BODMAS/PEMDAS).

Real-Time API Integration: Fetches live currency data via the FloatRates API using asynchronous HTTP requests.

Intelligent Input Validation: Prevents sequential operator entries (e.g., 8 + x -) and maintains expression integrity.

Automated Triggers: Real-time conversion triggering upon amount change or currency selection without additional button clicks.

Dynamic UI Navigation: Seamless transition between the standard calculator and the financial converter with state preservation.

🛠 Technical Stack / Kullanılan Teknolojiler
Technology,Usage Area
C++17,"Core logic, memory management, and OOP-based modular design."
Qt 6 Framework,GUI development (Widgets) and asynchronous event loop management.
Qt Network,RESTful API consumption using QNetworkAccessManager.
JSON Parsing,High-speed data deserialization with QJsonDocument.
CMake & Ninja,Cross-platform build orchestration and optimized compilation.

📁 Project Architecture / Proje Mimarisi
MainWindow: Manages the primary arithmetic engine and user interaction for standard calculations.

CurrentlyConverter: A specialized module handling the lifecycle of network requests, JSON parsing, and financial logic.

Signals & Slots: Implemented Qt’s core observer pattern for decoupled communication between UI components and the network manager.

Resource System (QRC): Embedded assets management for icons and design elements.

💻 Installation / Kurulum
Clone the repository:
git clone:  https://github.com/CerenSultanCETIN/ValutaMath-Hybrid-Financial-Calculator-with-Real-Time-Exchange
Open in Qt Creator: Select the CMakeLists.txt file.
Configure Build: Ensure the Network module is enabled in your Qt kit.
Run: Build and execute the application.

✍️ Developer's Note / Geliştirici Notu
This project was developed to demonstrate technical proficiency in asynchronous networking, API consumption, and Modern UI design. It reflects the engineering standards and technical rigor I have embraced as a Candidate Embedded Software Engineer at STG Mühendislik and as a student at Ostim Technical University.
