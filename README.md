# 💬 SO2\_Projekt – Serwer czatu wielopokojowego

---

## 🧠 Opis projektu

Projekt realizuje serwer czatu pozwalający na:

* rejestrację i logowanie użytkowników,
* tworzenie i obsługę wielu pokoi czatu,
* przesyłanie wiadomości między użytkownikami w ramach pokoju,
* zapisywanie i przesyłanie historii czatu dla każdego pokoju.

### Główne elementy projektu:

* Obsługa wielu klientów przez wątki (każdy klient otrzymuje osobny wątek).
* Synchronizacja dostępu do zasobów przy użyciu własnej implementacji SpinLock.
* Zapisywanie historii rozmów do plików.
* Autoryzacja użytkowników z prostym hashowaniem haseł.

---

## 🛠️ Kompilacja i uruchomienie

Projekt działa na systemie **Windows**, wykorzystując biblioteki WinSock2.

### 🔧 Kompilacja (CMake)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### ▶️ Uruchomienie

```bash
./build/SO2_ChatServer.exe
```

Serwer domyślnie nasłuchuje na porcie `23`.

---

## ⚙️ Opis działania

* Po połączeniu klient jest proszony o zalogowanie lub rejestrację.
* Po autoryzacji klient wybiera nazwę pokoju czatu.
* Jeśli pokój nie istnieje, zostaje automatycznie utworzony.
* Po dołączeniu klient otrzymuje historię pokoju.
* Wszelkie wiadomości są rozsyłane do pozostałych klientów w pokoju.
* Po rozłączeniu klient opuszcza pokój czatu.

---

## 🥝 Synchronizacja i wątki

| Element                 | Mechanizm                             |
| ----------------------- | ------------------------------------- |
| Obsługa klienta         | `std::thread`                         |
| Lista klientów w pokoju | `SpinLock` w klasie `ChatRoom`        |
| Lista pokoi             | `SpinLock` w klasie `ChatRoomManager` |
| Plik użytkowników       | `SpinLock` w klasie `UserManager`     |

### Użyty SpinLock

* Prosta implementacja oparta na `__sync_lock_test_and_set`.
* Zalecane tylko dla krótkich sekcji krytycznych.

---

## 📁 Struktura projektu

```
.
├── main.cpp             # Uruchamianie serwera
├── client_handler.*     # Obsługa pojedynczego klienta
├── user_manager.*       # Zarządzanie użytkownikami
├── chat_room.*          # Obsługa pojedynczego pokoju czatu
├── chat_room_manager.*  # Zarządzanie pokojami
├── SpinLock.h           # Implementacja spinlocka
├── data/                # Pliki użytkowników i historii czatu
└── README.md            # Dokumentacja projektu
```

---

## 🖋️ Uwagi końcowe

* Historia czatu zapisywana jest per pokój w katalogu `data/`.
* Obsługa WinSock2 wymaga inicjalizacji (WSAStartup/WSACleanup).

---

© 2025 | Systemy Operacyjne 2
