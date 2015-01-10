#include <iostream>
#include <memory>

namespace dsa
{
    inline namespace traversalMethod
    {
        enum class TraversalMethod
        {
            pre_order, post_order, in_order, breadth_first
        };
    }
    template< typename T >
    struct Node
    {
        std::unique_ptr< Node<T> > left, right;
        T value;
        
        constexpr Node(): left{ nullptr }, right{ nullptr }, value{} {}
        constexpr Node( T const & val ): left{ nullptr }, right { nullptr }, value { val } {}
        explicit operator T(){ return value; }
    };

    template< typename T, typename Less = std::less<T>>
    struct BinarySearchTree
    {
    private:
        std::unique_ptr< Node<T> > root;
        unsigned int _size;
        Less comp;
    public:
        constexpr BinarySearchTree(): root { nullptr }, _size( 0 ), comp{ } { }
        BinarySearchTree( std::initializer_list<T> lst, Less const &_comp = Less{} ): BinarySearchTree {}
        {
            _size = lst.size();
            comp = _comp;
            for( const auto &l : lst ) {
                insert( l );
            }
        }
        inline size_t size() const { return _size; }
        void insert( T const & val )
        {
            if( !root ) {
                root.reset( new Node<T> { val } );
            } else {
                insert_node( root.get(), val );
            }
        }
        void insert_node( Node<T> *ptr, T const & val )
        {
            if( comp( val, ptr->value ) ){
                if( !ptr->left ){
                    ptr->left.reset( new Node<T>{ val } );
                } else {
                    insert_node( ptr->left.get(), val );
                }
            } else {
                if( !ptr->right ) {
                    ptr->right.reset( new Node<T> { val } );
                } else {
                    insert_node( ptr->right.get(), val );
                }
            }
        }
        bool remove( T const & val )
        {
            if( !root ) return false;
            auto parentNode = find_parent( root.get(), val );
            auto nodeToRemove = find_node( root.get(), val );

            if( !nodeToRemove ) return false;
            if( size() == 1 ){ //We have one element in the BST
                root.reset();
                --_size;
                return true;
            } else if( !nodeToRemove->left && !nodeToRemove->right ) { //leaf node
                if( comp( nodeToRemove->value, parentNode->value ) ) {
                    parentNode->left.reset();
                } else {
                    parentNode->right.reset();
                }
            } else if( !nodeToRemove->left && nodeToRemove->right ) { //right subtree, no left subtree
                if( comp( nodeToRemove->value, parentNode->value )) {
                    parentNode->left = std::move( nodeToRemove->right );
                } else {
                    parentNode->right = std::move( nodeToRemove->right );
                }
            } else if( nodeToRemove->left && !nodeToRemove->right ) { //left subtree, no right subtree
                std::cout << "We are here" << std::endl;
                std::cout << parentNode->value << ", " << nodeToRemove->value << std::endl;
                if( comp( nodeToRemove->value, parentNode->value )) {
                    parentNode->right = std::move( nodeToRemove->left );
                } else {
                    parentNode->left.reset( nodeToRemove->left.release() );
                }
            } else { // Left and Right subtree available
                //~ 
            }
            --_size;
            return true;
        }
        

        inline void traverse( TraversalMethod meth = TraversalMethod::pre_order ){
            switch( meth ){
                case TraversalMethod::pre_order:
                    pre_order_traversal( root );
                    break;
                case TraversalMethod::post_order:
                    post_order_traversal( root );
                    break;
                case TraversalMethod::in_order:
                    in_order_traversal( root );
                    break;
                case TraversalMethod::breadth_first: default:
                    breadth_first_traversal( root );
            }
        }
        void in_order_traversal( std::unique_ptr<Node<T>> & n )
        {
            if( !n ) return;
            in_order_traversal( n->left );
            std::cout << ( T )*n << " ";
            in_order_traversal( n->right );
        }
        // ToDO
        void breadth_first_traversal( std::unique_ptr<Node<T>> & n )
        {
            if( !n ) return;
            /*
            breadth_first_traversal( n->visited );
            std::cout << n->value << " ";
            if (n->Left() != NULL) {
            	q.push(*n->Left());
            } if (n->Right() != NULL) {
            	q.push(*n->Right());
            }
            */
        }
        
        void post_order_traversal( std::unique_ptr<Node<T>> & n )
        {
            if( !n ) return;            
            post_order_traversal( n->left );
            post_order_traversal( n->right );
            std::cout << n->value << " ";
        }
        void pre_order_traversal( std::unique_ptr<Node<T>> & n ){
            if( !n ) return;
            std::cout << n->value << " ";
            pre_order_traversal( n->left );
            pre_order_traversal( n->right );
        }
        inline bool find( T const & val )
        {
            if( !root ) return false;

            return find_node( root.get(), val ) ? true: false;
        }
        
        void to_graphviz( std::ostream & os )
        {
            unsigned int index = 0;
            os << "digraph G\n";
            os << "{\n";
            if( root ) to_graphviz_impl( os, root, index );
            os << "}\n";
        }

        void to_graphviz_impl( std::ostream & os, std::unique_ptr<Node<T>> &ptr, unsigned int &index )
        {
            if( !ptr ) return;

            unsigned int current_index = index;
            
            os << "NODE" << current_index << " [ label = \"" << ptr->value << "\"]\n";

            ++index;
            if( ptr->left ){
                os << "NODE" << current_index << " -> NODE" << index << "\n";
                to_graphviz_impl( os, ptr->left, index );
            }
            if( ptr->right ){
                os << "NODE" << current_index << " -> NODE" << index << "\n";
                to_graphviz_impl( os, ptr->right, index );
            }
        }
        
        Node<T> *find_node( Node<T>* ptr, T const & val )
        {
            if( !ptr ) return nullptr;
            if( ptr->value == val ) return ptr;
            
            if( comp( val, ptr->value )) return find_node( ptr->left.get(), val );
            else return find_node( ptr->right.get(), val );
        }
         
        Node<T>* find_parent( Node<T> *ptr, T const & val )
        {
            if( !ptr ) return nullptr;
            if( val == ptr->value ) return nullptr;

            if( comp( val, ptr->value )) {
                if( !ptr->left ) {
                    return nullptr;
                } else if ( val == ptr->left->value ) {
                    return ptr;
                } else {
                    return find_parent( ptr->left.get(), val );
                }
            } else {
                if( !ptr->right ) {
                    return nullptr;
                } else if( val == ptr->right->value ) {
                    return ptr;
                } else {
                    return find_parent( ptr->right.get(), val );
                }
            }
            return nullptr;
        }
        std::tuple<bool, T> find_min() const
        {
            if( !root ) return std::make_tuple( false, T{} );
            return find_min( root.get() );
        }
        
        std::tuple<bool, T> find_min( Node<T> * ptr ) const
        {
            return !ptr->left ? std::make_tuple( true, ptr->value ) : find_min( ptr->left.get() );
        }
        std::tuple<bool, T> find_max() const
        {
            if( !root ) return std::make_tuple( false, T{} );
            return find_max( root.get() );
        }
        
        std::tuple<bool, T> find_max( Node<T> * ptr ) const
        {
            return !ptr->right ? std::make_tuple( true, ptr->value ) : find_max( ptr->right.get() );
        }
    };
}

struct Foo
{
    int _a;
    Foo( int const & a ): _a( a ) {}
    friend bool operator==( Foo const &a, Foo const & f )
    {
        return a._a == f._a;
    }
    friend std::ostream& operator<<( std::ostream & os, Foo const & a )
    {
        return os << a._a;
    }
};

bool compare( Foo const & a, Foo const & f ){
    return a._a < f._a;
}

template< typename T, typename Compare >
using bst = dsa::BinarySearchTree<T, Compare>;

int main()
{
    bst<Foo, decltype( compare )*> _bst ( { 23, 11 }, compare );
    _bst.traverse( dsa::TraversalMethod::post_order );
    std::endl( std::cout );
    _bst.traverse( dsa::TraversalMethod::in_order );
    _bst.remove( 23 ); //TODO
    std::endl( std::cout );
    _bst.traverse();
    
    
    return 0;
}
