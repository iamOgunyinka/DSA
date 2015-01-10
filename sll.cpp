/* Singly Linked List by Ogunyinka Josh */

#include <iostream>
#include <memory>

namespace dsa { // Data Structure and Algorithms

    template< typename Type >
    struct Node
    {
        std::shared_ptr< Node <Type> > next { nullptr };
        Type value { };
    };
    
    template< typename T >
    struct SLList // Singly Linked List
    {
    private:
        std::shared_ptr< Node<T> > head, tail;
        int _size;
    public:
        ~SLList() = default;
        SLList( ): head( nullptr ), tail( nullptr ), _size ( 0 ) { }
        SLList( std::initializer_list<T> lst ): SLList {} {
            for( const auto &l: lst ){
                push_back( l );
            }
        }
    
    public:
        void push_back( T const & val ); // O( 1 )
        std::pair< bool, Node<T> *> find( T const & val ) const ; // O( n )
        bool erase( T const & val ); // O( 1 )
        void traverse() const ; // O( n )
        void reverse_traverse() const; // O( n^2 )
        int size() const { return _size; } // O( 1 )
    };

    template< typename T >
    std::pair< bool, Node<T> *> SLList<T>::find( T const & val ) const
    {
        if( !head ) return { false, nullptr };
        
        Node<T> *n = head.get();
        while( n && n->value != val ) {
            n = n->next.get();
        }
        if( !n ) return { false, nullptr };
        return { true, n };
    }
    
    template< typename T >
    void SLList<T>::traverse( ) const
    {
        if( !head ) return;
        
        std::shared_ptr< Node<T> > temp = head;
        while( temp ) {
            std::cout << temp->value << ", ";
            temp = temp->next;
        }
    }
    
    template< typename T >
    void SLList<T>::reverse_traverse( ) const
    {
        if ( !tail ) return;
        auto curr = tail;

        while( curr != head ) {
            auto prev = head;
            while( prev->next != curr ){
                prev = prev->next;
            }
            std::cout << curr->value << ", ";
            curr = prev;
        }
        std::cout << curr->value << std::endl;
    }    
    template< typename T >
    void SLList<T>::push_back( T const & val )
    {
        if( !head ) {
            head.reset( new Node<T> );
            head->value = val;
            tail = head;
        } else {
            tail->next.reset( new Node<T> );
            tail->next->value = val;
            tail = tail->next;
        }
        ++_size;
    }
    template< typename T >
    bool SLList<T>::erase( T const & val )
    {
        if ( !head ) return false;
        Node<T> *n = head.get();

        if( n->value == val ){ //Best case scenario O( 1 )
            if( head == tail ){
                head.reset();
                tail.reset();
            } else {
                head = head->next;
            }
            --_size;
            return true;
        }
        while( n->next && n->next->value != val ) {
            n = n->next.get();
        }
        if( n ) {
            if( n->next ){
                if( n->next == tail ){
                    tail.reset();
                }
                n->next = n->next->next; // de-allocation is automatic
                --_size;
                return true;
            }
        }
        return false;
    }
}

int main()
{
    dsa::SLList< int > list, list_2 { 20, 21, 44, 24, 46, 67, 63, 678, 11 };
    list_2.traverse();
    std::endl( std::cout );
    
    list_2.erase( 11 );
    list_2.traverse();

    for( int i = 1; i != 51; ++i )
        list.push_back( i * 10 );
    auto found = list.find( 50 );
    if( found.first ) {
        std::cout << std::endl << found.second->value << " found." << std::endl;
    }
    std::cout << "Size of list is " << list.size() << std::endl;
    int i = 10;
    while( i <= 400 ) {
        list.erase( i );
        i += 10;
    }
    list.push_back( i ); // add 410 to the list
    std::cout << std::endl << "Size of list is " << list.size() << ", and list_2 is " << list_2.size() << std::endl;
    list.traverse();
    std::cout << "\nReversed, I have this: ";
    list.reverse_traverse();
    return 0;
}
