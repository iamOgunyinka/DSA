/* Double Linked List by Ogunyinka Josh */

#include <iostream>
#include <memory>

namespace dsa
{
    template< typename T >
    struct Node
    {
        Node() = default;
        Node( T const & val ): prev { nullptr }, next { nullptr }, value{ val } { }

        std::shared_ptr< Node<T> > prev { nullptr }, next { nullptr };
        T value { };
    };

    template< typename T >
    struct DLList
    {
        DLList(): head{ nullptr }, tail { nullptr }, _size{ 0 } { }
        DLList( std::initializer_list<T> lst ): DLList{} {
            for( const auto &l : lst ){
                push_back( l );
            }
        }
        ~DLList() = default;

        void push_back( T const & val ); // O( 1 )
        void traverse() const ; // O( n )
        void reverse_traverse() const; // O( n )
        bool find( T const & val ) const; // O( n )
        size_t size() const { return _size; } // O( 1 )
        bool erase( T const & val ); // O( n )
    private:
        std::shared_ptr< Node <T> > head, tail;
        size_t _size;
    };
}

template< typename T >
bool dsa::DLList<T>::find( T const & val ) const
{
    if( !head ) return false;

    auto curr = head;
    while( curr && curr->value != val ){
        curr = curr->next;
    }
    if( !curr ) return false;
    return true;
}

template< typename T >
void dsa::DLList<T>::traverse( ) const
{
    auto curr = head;
    while( curr ){
        std::cout << curr->value << ", ";
        curr = curr->next;
    }
    std::endl( std::cout );
}

template< typename T >
void dsa::DLList<T>::reverse_traverse() const
{
    auto curr = tail;
    while( curr ){
        std::cout << curr->value << ", ";
        curr = curr->prev;
    }
}

template< typename T >
void dsa::DLList<T>::push_back( T const & val )
{
    std::shared_ptr< Node<T> > n = std::make_shared< Node<T> > ( val );
    if ( !head ) {
        head = n;
        tail = head;
    } else {
        n->prev = tail;
        tail->next = n;
        tail = n;
    }
    ++_size;
}

template< typename T >
bool dsa::DLList<T>::erase( T const & val )
{
    if( !head ) return false;
    if( head->value == val ) {
        if( head == tail ) {
            head.reset();
            tail.reset();
        } else {
            head = head->next;
            head->prev.reset();
        }
        --_size;
        return true;
    }
    std::shared_ptr< Node<T> > n = head->next;
    while( n && n->value != val ){
        n = n->next;
    }

    if( n ){
        if( n == tail ) {
            tail = tail->prev;
            tail->next.reset();
        } else {
            n->prev->next = n->next;
            n->next->prev = n->prev;
        }
        --_size;
        return true;
    }
    return false;
}

int main()
{
    dsa::DLList< std::string > fs { "AO", "BP", "CD", "EF", "GH", "IJ", "KL", "MN" };
    std::cout << ( fs.traverse(), fs.size() ) << std::endl; // Do not try this at home.

    fs.erase( "AO" );
    fs.traverse();

    fs.erase( "CD" );
    fs.traverse();
    
    fs.erase( "MN" );
    fs.traverse();

    fs.erase( "IJ" );
    fs.reverse_traverse();
    std::cout << fs.size() << std::endl;
    return 0;
}
