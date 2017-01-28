#pragma once
namespace ul
{
	// -------------------------------------------------
	// cyclic buffer for printing
	// -------------------------------------------------
	template <size_t sz> struct stream_arena 
	{
		void put_byte(unsigned char c) {
			*end_++ = c;
			if (end_ == begin_) shift(begin_); else shift(end_);
		}
		unsigned char get_byte(void) {
			if (begin_ == end_) return 0;
			auto result = *begin_++;
			shift(begin_);
			return result;
		}
		bool empty() const {
			return (begin_ == end_);
		}
		size_t available() const {
			return (begin_ <= end_) ? end_ - begin_ : (right_margin_ - begin_) + (end_ - left_margin_);
		}
		const unsigned char * get_buffer(size_t& len) {
			len = (begin_ <= end_) ? end_ - begin_ : (right_margin_ - begin_);
			auto result = begin_;
			begin_ += len;
			shift(begin_);
			return result;
		}
	protected:
		unsigned char * const left_margin_ = new unsigned char[sz];
		unsigned char * const right_margin_ = left_margin_ + sz;
		unsigned char * begin_ = left_margin_;
		unsigned char * end_ = left_margin_;
	protected:
		void shift(unsigned char*& p) { if (p == right_margin_) p = left_margin_; }
	};
	
}

