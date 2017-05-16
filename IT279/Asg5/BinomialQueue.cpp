#include "BinomialQueue.h"
#include <iostream>

static const int MAX = 15;
template <class T>
BinomialQueue<T>::BinomialQueue():myQueue(MAX)
{
   for(int i = 0; i < myQueue.size();i++)
	   myQueue[ i ] = NULL;
   currentSize = 0;
}

template <class T>
BinomialQueue<T>::~BinomialQueue()
{
   makeEmpty();
}

template <class T>
void BinomialQueue<T>::merge(BinomialQueue<T> & rhs)
{

   if(this == &rhs)    
	   return;

   if(currentSize+rhs.currentSize > capacity())
   {
		cout << "The capacity has been exceeded" << endl;
		return;
   }  
			
   currentSize += rhs.currentSize;
   BinomialNode<T> *carry = NULL;

   for(int i = 0,j = 1; j <= currentSize; i++,j*=2)
   {
	   BinomialNode<T> *t1 = myQueue[i];
	   BinomialNode<T> *t2 = rhs.myQueue[i];

	   int condition = (t1 == NULL)?0:1;
	   condition += (t2 == NULL)?0:2;
	   condition += (carry == NULL)?0:4;

	   switch(condition)
	   {
		   case 0: 
		   case 1: 
			   break;
		   case 2: 
			   myQueue[i] = t2;
			   rhs.myQueue[i] = NULL;
			   break;
		   case 4: 				
			   myQueue[i] = carry;
			   carry = NULL;
			   break;
		   case 3: 
			   carry = merge(t1,t2);
			   myQueue[i] = rhs.myQueue[i] = NULL;
			   break;
		   case 5: 
			   carry = merge(t1,carry);
			   myQueue[i] = NULL;
			   break;
		   case 6: 
			   carry = merge(t2,carry);
			   rhs.myQueue[i] = NULL;
			   break;
		   case 7: 
			   myQueue[i] = carry;
			   carry = merge(t1,t2);
			   rhs.myQueue[i] = NULL;
			   break;
	   }
   }

   for(int k = 0;k < rhs.myQueue.size();k++)
	   rhs.myQueue[k] = NULL;
   rhs.currentSize = 0;
 }        

//merge two binomial nodes, then heap sort while maintaining heap order
template <class T>
BinomialNode<T> *BinomialQueue<T>::merge(BinomialNode<T> *h1,BinomialNode<T> *h2) const
{
   if(h1 == NULL)
	   return h2;
   if(h2 == NULL)
	   return h1;

   if(h1->element < h2->element)
	   return merge1(h1,h2);
   else
	   return merge1(h2,h1);
}


template <class T>
BinomialNode<T> *BinomialQueue<T>::merge1(BinomialNode<T> * h1,BinomialNode<T> * h2) const
{
	 if(h1->leftChild == NULL)   
		  h1->leftChild = h2; 
	 else
	 {
		  h1->nextSibling = merge(h1->nextSibling,h2);
		  swapChildren(h1);
	 }
	 return h1;
}


//swap the left child and next sibling
template <class T>
void BinomialQueue<T>::swapChildren(BinomialNode<T> *t) const
{
	BinomialNode<T> *tmp = t->leftChild;
		t->leftChild = t->nextSibling;
	t->nextSibling = tmp;
}

//insert the item while maintaining the heap order
template <class T>
void BinomialQueue<T>::insert(const T & elem)
{
	BinomialQueue newQ;
	newQ.currentSize = 1;
	newQ.myQueue[0] = new BinomialNode<T>(elem, NULL, NULL);
	merge(newQ);
}

template <class T>
bool BinomialQueue<T>::isEmpty() const
{
	return (currentSize == 0);
}

template <class T>
bool BinomialQueue<T>::isFull() const
{
	return currentSize == capacity();
}

template <class T>
void BinomialQueue<T>::makeEmpty()
{
	currentSize = 0;
	for(int i = 0; i < myQueue.size(); i++)
		makeEmpty(myQueue[i]);
}

template <class T> 
int BinomialQueue<T>::capacity() const
{
	return (1 << myQueue.size())-1;
}

template <class T>
void BinomialQueue<T>::makeEmpty(BinomialNode<T> * & t) const
{
			if(t != NULL)
			{
				makeEmpty(t->leftChild);
				makeEmpty(t->nextSibling);
				delete t;
				t = NULL;
			}
}

template <class T>
void BinomialQueue<T>::print() 
{
	int i, maxIndex = 0;

	for(i = 0; i < myQueue.size(); i++)
	{
		if (myQueue[ i ] != NULL) 
			maxIndex = i;
	}
	for(int i = 0; i <= maxIndex; i++)
	{
			BinomialNode<T> *node = myQueue[i];
		if (node != NULL)
			{
			cout << "B[" << i << "]:  ";
			node->printElement(0);
			node->printChild(1);
			node->printSibling(1);
			cout << endl; 
			}
		else
			{
			cout << "B[" << i << "]:   ----------------" << endl; 
			cout << endl;
			} 
		}
}
