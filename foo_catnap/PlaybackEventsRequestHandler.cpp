#include "stdafx.h"

#include "Poco/AutoPtr.h"
#include "Poco/AbstractObserver.h"
#include "Poco/NObserver.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/NumberParser.h"
#include "Poco/SharedPtr.h"
#include "Poco/Thread.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "Poco/JSON/Stringifier.h"

#include "PlaybackEventsRequestHandler.h"
#include "PlaybackFacade.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace CatNap {
	template<typename N>
	class PostingObserver : public Poco::AbstractObserver
	{
	public:
		typedef Poco::AutoPtr<N> NotificationPtr;
		typedef Poco::SharedPtr<Poco::NotificationQueue> NotificationQueuePtr;

		PostingObserver(const NotificationQueuePtr &pQueue):
			_pQueue(pQueue)
		{
		}

		PostingObserver(const PostingObserver& observer):
			AbstractObserver(observer),
			_pQueue(observer._pQueue)
		{
		}

		~PostingObserver()
		{
		}

		PostingObserver& operator = (const PostingObserver& observer)
		{
			if (&observer != this)
			{
				_pQueue = observer._pQueue;
			}
			return *this;
		}
	
		void notify(Poco::Notification* pNf) const
		{
			Poco::Mutex::ScopedLock lock(_mutex);

			if (_pQueue)
			{
				N* pCastNf = dynamic_cast<N*>(pNf);
				if (pCastNf)
				{
					NotificationPtr ptr(pCastNf, true);
					_pQueue->enqueueNotification(ptr);
				}
			}
		}
	
		bool equals(const Poco::AbstractObserver& abstractObserver) const
		{
			const PostingObserver* pObs = dynamic_cast<const PostingObserver*>(&abstractObserver);
			return pObs && pObs->_pQueue == _pQueue;
		}

		bool accepts(Poco::Notification* pNf) const
		{
			return dynamic_cast<N*>(pNf) != 0;
		}
	
		Poco::AbstractObserver* clone() const
		{
			return new PostingObserver(*this);
		}
	
		void disable()
		{
			Poco::Mutex::ScopedLock lock(_mutex);
		
			_pQueue = 0;
		}

	private:
		PostingObserver();

		mutable NotificationQueuePtr _pQueue;
		mutable Poco::Mutex _mutex;
	};

	PlaybackEventsRequestHandler::PlaybackEventsRequestHandler(PlaybackFacade &facade):
		_facade(facade)
	{
	}

	void PlaybackEventsRequestHandler::run()
	{
		if (request().getMethod() == HTTPRequest::HTTP_HEAD || request().getMethod() == HTTPRequest::HTTP_GET)
		{
			if (request().has("Accept") && (request().get("Accept") == "text/event-stream")) {
				unsigned lastEventId = 0;
				if (request().has("Last-Event-ID")) {
					// Process value of Last-Event-ID as base 10 number
					if (!Poco::NumberParser::tryParseUnsigned(request().get("Last-Event-ID"), lastEventId)) {
						lastEventId = 0;
					}
				}

				response().setKeepAlive(true);
				response().setContentType("text/event-stream");
				response().set("Cache-Control", "no-cache");

				std::ostream& out = response().send();
				if (request().getMethod() == HTTPRequest::HTTP_GET)
				{
					Poco::SharedPtr<Poco::NotificationQueue> pQueue = new Poco::NotificationQueue();

					PostingObserver<Poco::Notification> observer = PostingObserver<Poco::Notification>(pQueue);

					_facade.addObserver(observer);

					out << std::flush;

					try
					{
						const long timeout = 1000L;
						Poco::Notification::Ptr pNf = pQueue->waitDequeueNotification(timeout);
						while (true)
						{
							if (pNf && dynamic_cast<PlaybackNotification*>(pNf.get()))
							{
								PlaybackNotification *pPbNf = dynamic_cast<PlaybackNotification*>(pNf.get());
								sendPlaybackEvent(out, *pPbNf);
							}
							else
							{
								// Send keep-alive
								out << "\r\n";
								out << std::flush;
							}

							pNf = pQueue->waitDequeueNotification(timeout);
						}
					}
					catch (...)
					{
						_facade.removeObserver(observer);
						throw;
					}
				}
			} else {
				sendErrorResponse(HTTPResponse::HTTP_NO_CONTENT, "");
			}
		}
		else
		{
			sendErrorResponse(HTTPResponse::HTTP_METHOD_NOT_ALLOWED, "");
		}
	}

	void PlaybackEventsRequestHandler::sendPlaybackEvent(std::ostream &out, const PlaybackNotification &nf)
	{
		out << "data: {";
		out << "\"started\":" << (nf.isStarted() ? "true" : "false");
		out << ",\"paused\":" << (nf.isPaused() ? "true" : "false");
		out << ",\"track\":";
		if (nf.isMapValid()) {
			out << "{";
			std::string name;
			const Poco::ListMap<std::string, std::string> &map = nf.map();
			for (auto it = map.begin(); it != map.end(); ++it)
			{
				if (it->first != name)
				{
					if (!name.empty())
					{
						out << "],";
					}
					name = it->first;
					Poco::JSON::Stringifier::formatString(name, out);
					out << ":[";
				}
				else
				{
					out << ",";
				}
				Poco::JSON::Stringifier::formatString(it->second, out);
			}
			if (!name.empty())
			{
				out << "]";
			}
			out << "}";
		} else {
			out << "null";
		}
		out << "}\r\n";
		out << "\r\n";
		out << std::flush;
	}

} // namespace CatNap
