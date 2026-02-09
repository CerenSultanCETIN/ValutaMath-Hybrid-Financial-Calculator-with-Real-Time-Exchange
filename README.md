📊 FinCalc-Live: Hybrid Financial Calculator & Real-Time Exchange Engine
FinCalc-Live is a high-performance, hybrid desktop application developed with C++17 and Qt 6 Framework. It seamlessly integrates a traditional mathematical engine with an asynchronous financial data system. Inspired by Apple's minimalist aesthetic, it provides real-time currency conversion alongside complex arithmetic capabilities.

🚀 Key Features / Öne Çıkan Özellikler
Hybrid Calculation Engine: Perform complex arithmetic with paranthesis support and operator precedence (BODMAS/PEMDAS). / Karmaşık işlemleri parantez desteği ve işlem önceliğiyle çözer.

Live Currency Conversion: Fetches real-time exchange rates via FloatRates API using asynchronous HTTP requests. / FloatRates API üzerinden asenkron HTTP istekleri ile anlık kur verilerini çeker.

Intelligent Input Validation: Smart algorithms to prevent sequential operator entries (e.g., 8 + x -) and ensure valid mathematical expressions. / Ard arda operatör girişini engelleyen akıllı kontrol mekanizması.

Automated Triggers: Instant conversion updates upon value changes or currency selection—no extra button clicks required. / Miktar veya birim değiştiği anda otomatik döviz hesaplama.

Dynamic UI Navigation: Smooth transitions between the standard calculator and the financial converter with state preservation. / Modüller arasında akıcı geçiş ve form yönetimi.

🛠 Technical Stack / Kullanılan Teknolojiler
Language: C++17 (Modern OOP Principles)

Framework: Qt 6.10 (Core, Widgets, Network modules)

Asynchronous Networking: Utilized QNetworkAccessManager for non-blocking API communication.

Data Parsing: High-speed JSON processing with QJsonDocument and QJsonObject.

Build System: CMake & Ninja for optimized compilation.

📁 Project Architecture / Proje Mimarisi
MainWindow: Handles the primary arithmetic logic and standard calculator interface.

CurrentlyConverter: Manages RESTful API interactions, currency logic, and hybrid calculation integration.

Signals & Slots: Implemented Qt’s core observer pattern for robust asynchronous event management.

💻 Installation / Kurulum
Clone the repository: git clone https://github.com/yourusername/FinCalc-Live.git

Open CMakeLists.txt in Qt Creator.

Ensure your kit includes the Network module.

Build and Run.

✍️ Developer's Note / Geliştirici Notu
This project was developed to showcase advanced skills in asynchronous networking, API consumption, and Modern UI/UX design. It represents the engineering standards and technical rigor I have cultivated during my tenure as a Candidate Embedded Software Engineer at STG Mühendislik.
