#pragma once

namespace IceFairy {

	template<class... Ts>
	class JobSystem {
	public:
		template<class... Ms>
		JobSystem(Ms&&... metadata) { }

		virtual void Execute(std::shared_ptr<Ts>&... components) { }
	};

}
