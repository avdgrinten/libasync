#ifndef ASYNC_JUMP_HPP
#define ASYNC_JUMP_HPP

#include <mutex>
#include <deque>

#include <async/result.hpp>
#include <async/doorbell.hpp>
#include <boost/intrusive/list.hpp>
#include <cofiber.hpp>

namespace async {

struct jump {
	struct awaiter : boost::intrusive::list_base_hook<> {
		friend struct jump;

	private:
		explicit awaiter(jump *owner)
		: _owner{owner} { }

	public:
		bool await_ready() {
			return false;
		}

		void await_suspend(cofiber::coroutine_handle<void> handle) {
			_coro = handle;

			bool done;
			{
				std::lock_guard<std::mutex> lock(_owner->_mutex);
				done = _owner->_done;
				if(!done)
					_owner->_waiters.push_back(*this);
			}

			if(done)
				_coro.resume();
		}

		void await_resume() {
			// Just return void.
		}

	private:
		jump *_owner;
		cofiber::coroutine_handle<void> _coro;
	};

	jump()
	: _done(false) { }

	void trigger() {
		boost::intrusive::list<awaiter> items;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_done = true;
			items.splice(items.end(), _waiters);
		}

		// Invoke the callbacks without holding locks.
		while(!items.empty()) {
			auto item = &items.front();
			items.pop_front();
			item->_coro.resume();
		}
	}

	// TODO: This emulation is a bad idea from a performance point-of-view. Refactor it.
	awaiter async_wait() {
		return awaiter{this};
	}

	void reset() {
		// TODO: We do not need the lock for exclusion.
		// Do we need it for the memory barrier?
		std::lock_guard<std::mutex> lock(_mutex);
		_done = false;
	}

private:
	std::mutex _mutex;
	bool _done;
	boost::intrusive::list<awaiter> _waiters;
};

} // namespace async

#endif // ASYNC_JUMP_HPP
