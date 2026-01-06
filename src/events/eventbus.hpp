#ifndef EVENTBUS_HPP
#define EVENTBUS_HPP

#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>

class EventBus {
private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> handlers;
    
    template<typename T>
    std::type_index getTypeIndex() const {
        return std::type_index(typeid(T));
    }

public:
    template<typename T>
    void subscribe(std::function<void(const T&)> handler) {
        auto typeIndex = getTypeIndex<T>();
        handlers[typeIndex].push_back([handler](const void* event) {
            handler(*static_cast<const T*>(event));
        });
    }
    
    template<typename T>
    void emit(const T& event) {
        auto typeIndex = getTypeIndex<T>();
        auto it = handlers.find(typeIndex);
        if (it != handlers.end()) {
            for (auto& handler : it->second) {
                handler(&event);
            }
        }
    }
    
    void clear() {
        handlers.clear();
    }
};

#endif // EVENTBUS_HPP

